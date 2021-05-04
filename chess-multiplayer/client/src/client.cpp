#include "../include/client.h"
#include <sstream>


void client::mainloop()
{
    bool running = true;

    asio::io_service service;
    tcp::socket sock(service);

    Color my_color{ Color::WHITE };

    std::mutex mtx;

    if (!connect_to_server(sock, my_color))
    {
        system("pause");
        return;
    }

    core::init();
    SDL_Event evt;

    SDL_SetWindowTitle(core::m_gfx->window(), (std::string("chess | ") + (my_color == Color::WHITE ? "white" : "black")).c_str());

    bool waiting = true;

    while (waiting)
    {
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                return;
            }
        }

        if (sock.available() > 0)
        {
            std::string data = sock_read(sock);
            std::string type = core::multiplayer::get_elem_from_string(data, "type");

            if (type == "game-start")
            {
                waiting = false;
                std::cout << "found second player, game is starting\n";
            }
        }

        SDL_RenderClear(core::m_gfx->rend());

        core::utils::draw_text(core::m_gfx.get(), core::m_font, "waiting for another player", 300, 475);

        SDL_RenderPresent(core::m_gfx->rend());
    }

    std::thread thr_recv(receive, std::ref(sock), std::ref(mtx), std::ref(running));
    setup_board();

    int prev_x, prev_y;
    SDL_GetMouseState(&prev_x, &prev_y);

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (core::m_turn == my_color)
                    core::handle_mouse(prev_x, prev_y, true);

                break;
            case SDL_MOUSEBUTTONUP:
                if (core::m_turn == my_color)
                    core::handle_mouse(prev_x, prev_y, false);

                break;
            }
        }

        std::string newest_change = core::multiplayer::get_new_changes();
        send(sock, newest_change);

        core::piece_follow_cursor(prev_x, prev_y);

        SDL_GetMouseState(&prev_x, &prev_y);

        core::clear_and_draw();
    }

    if (thr_recv.joinable()) thr_recv.join();

    core::cleanup();
}


void client::receive(tcp::socket& sock, std::mutex& mtx, bool& running)
{
    while (running)
    {
        if (sock.available() > 0)
        {
            std::string data = sock_read(sock);

            std::cout << "received " << data.size() << " bytes\n";

            std::string type = core::multiplayer::get_elem_from_string(data, "type");

            if (type == "new-move")
                handle_new_move(data);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void client::send(tcp::socket& sock, const std::string& msg)
{
    sock.write_some(asio::buffer(msg.data(), msg.size()));
}


void client::setup_board()
{
    for (int i = 0; i < 8; ++i)
    {
        core::new_piece(PieceType::PAWN, Color::BLACK, i, 1);
        core::new_piece(PieceType::PAWN, Color::WHITE, i, 6);
    }

    std::string format = "rkbQKbkr";

    for (int i = 0; i < format.size(); ++i)
    {
        char c = format[i];

        PieceType type = PieceType::PAWN;

        switch (c)
        {
        case 'r': type = PieceType::ROOK; break;
        case 'k': type = PieceType::KNIGHT; break;
        case 'b': type = PieceType::BISHOP; break;
        case 'K': type = PieceType::KING; break;
        case 'Q': type = PieceType::QUEEN; break;
        }

        core::new_piece(type, Color::BLACK, i, 0);
        core::new_piece(type, Color::WHITE, i, 7);
    }
}


bool client::connect_to_server(tcp::socket& sock, Color& color)
{
    asio::error_code ec;

    std::cout << "server ip: ";
    std::string server_ip;
    std::getline(std::cin, server_ip);

    sock.connect(tcp::endpoint(address::from_string(server_ip), 1234), ec);

    if (ec)
        std::cout << "error: " << ec.message() << "\n";
    else
        std::cout << "connected to server\n";

    std::cout << "waiting for response from server\n";

    while (sock.available() == 0)
    {
        std::cout << "checking again\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "read " << sock.available() << " bytes\n";

    std::string data = sock_read(sock);

    if (core::multiplayer::get_elem_from_string(data, "type") == "error")
    {
        std::cout << "error: " << core::multiplayer::get_elem_from_string(data, "message") << "\n";
        return false;
    }

    color = (core::multiplayer::get_elem_from_string(data, "color") == "white" ? Color::WHITE : Color::BLACK);

    std::cout << "received response\n";
    std::cout << "your color is " << core::multiplayer::get_elem_from_string(data, "color") << "\n";

    return true;
}


void client::handle_new_move(const std::string& data)
{
    core::m_turn = (core::m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);

    switch (core::m_turn)
    {
    case Color::BLACK: std::cout << "changed turn from white to black\n"; break;
    case Color::WHITE: std::cout << "changed turn from black to white\n"; break;
    }


    SDL_Point orig = {
        std::stoi(core::multiplayer::get_elem_from_string(data, "px")),
        std::stoi(core::multiplayer::get_elem_from_string(data, "py"))
    };

    SDL_Point curr = {
        std::stoi(core::multiplayer::get_elem_from_string(data, "x")),
        std::stoi(core::multiplayer::get_elem_from_string(data, "y"))
    };

    Piece* p = core::piece_at(orig.x, orig.y);
    Piece* eaten = core::piece_at(curr.x, curr.y);

    if (eaten)
        core::remove_piece(eaten);

    p->grid_move_to(curr.x, curr.y);

    core::check_kings();
}


std::string client::sock_read(tcp::socket& sock)
{
    std::vector<char> buf(sock.available());
    sock.read_some(asio::buffer(buf.data(), buf.size()));

    std::string data;
    for (char c : buf)
        data += c;

    return data;
}