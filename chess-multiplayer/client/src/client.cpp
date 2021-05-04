#include "../include/client.h"
#include <sstream>


void client::mainloop()
{
	bool running = true;

	asio::error_code ec;

	asio::io_service service;
	tcp::socket sock(service);

	Color my_color{ Color::WHITE };

	std::mutex mtx;
	connect_to_server(sock, my_color);
	std::thread thr_recv(receive, std::ref(sock), std::ref(mtx), std::ref(running));

	core::init();

	setup_board();

	SDL_Event evt;

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
			std::vector<char> buf(sock.available());
			sock.read_some(asio::buffer(buf.data(), buf.size()));

			std::string data;
			for (char c : buf)
				data += c;

			std::cout << "received: " << data << "\n";

			std::string type = core::multiplayer::get_elem_from_string(data, "type");

			if (type == "new-move")
			{
				core::m_turn = (core::m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);

				switch (core::m_turn)
				{
				case Color::BLACK: std::cout << "changed turn from white to black\n"; break;
				case Color::WHITE: std::cout << "changed turn from black to white\n"; break;
				}
			}
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


void client::connect_to_server(tcp::socket& sock, Color& color)
{
	asio::error_code ec;

	std::cout << "server ip: ";
	std::string server_ip;
	std::getline(std::cin, server_ip);

	sock.connect(tcp::endpoint(address::from_string(server_ip), 1234), ec);

	if (ec)
		std::cout << "error: " << ec.message() << "\n";
	else
		std::cout << "connected successfully\n";

	while (sock.available() == 0)
		std::cout << "\rwaiting for response from server...";

	std::cout << "\n";

	std::vector<char> buf(sock.available());
	sock.read_some(asio::buffer(buf.data(), buf.size()));

	std::string data;
	for (char c : buf)
		data += c;

	color = (core::multiplayer::get_elem_from_string(data, "color") == "white" ? Color::WHITE : Color::BLACK);

	std::cout << "received response\n";
	std::cout << "your color is " << core::multiplayer::get_elem_from_string(data, "color") << "\n";
}