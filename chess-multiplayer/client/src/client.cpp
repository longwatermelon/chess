#include "../include/client.h"
#include <sstream>


void client::mainloop()
{
	asio::error_code ec;

	asio::io_service service;
	tcp::socket sock(service);

	std::cout << "server ip: ";
	std::string server_ip;
	std::getline(std::cin, server_ip);

	sock.connect(tcp::endpoint(address::from_string(server_ip), 1234), ec);

	if (ec)
		std::cout << "error: " << ec.message() << "\n";
	else
		std::cout << "connected successfully\n";


	core::init();

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

	bool running = true;
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
				core::handle_mouse(prev_x, prev_y, true);
				break;
			case SDL_MOUSEBUTTONUP:
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

	core::cleanup();
}


void client::receive(tcp::socket& sock, std::mutex& mtx, bool& running)
{
}


void client::send(tcp::socket& sock, const std::string& msg)
{
	sock.write_some(asio::buffer(msg.data(), msg.size()));
}