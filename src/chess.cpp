#include "../include/chess.h"
#include <iostream>


void chess::init()
{
	std::cout << "loading pieces\n";

	m_gfx = std::make_unique<Graphics>("chess");
	m_selected_piece = nullptr;

	for (int i = 0; i < 8; ++i)
	{
		m_pieces.emplace_back(Piece(PieceType::PAWN, Color::BLACK, i, 1, m_gfx.get()));
		m_pieces.emplace_back(Piece(PieceType::PAWN, Color::WHITE, i, 6, m_gfx.get()));
	}

	std::string format = "rkbKQbkr";

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

		m_pieces.emplace_back(Piece(type, Color::BLACK, i, 0, m_gfx.get()));
		m_pieces.emplace_back(Piece(type, Color::WHITE, i, 7, m_gfx.get()));
	}

	std::cout << "finished\n";
}


void chess::mainloop()
{
	bool running = true;
	SDL_Event evt;

	bool mouse_down = false;

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
				mouse_down = true;
				handle_mouse(prev_x, prev_y, true);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_down = false;
				handle_mouse(prev_x, prev_y, false);
				break;
			}
		}

		if (m_selected_piece)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			m_selected_piece->move(x - prev_x, y - prev_y);
		}

		SDL_GetMouseState(&prev_x, &prev_y);

		m_gfx->clear();

		draw_board();

		m_gfx->color({ 0, 255, 0 });
		for (auto& p : m_valid_moves)
		{
			SDL_Rect rect{ p.x * 100 + 100, p.y * 100 + 100, 100, 100 };
			SDL_RenderFillRect(m_gfx->rend(), &rect);
		}
		m_gfx->color({ 0, 0, 0 });

		for (auto& piece : m_pieces)
		{
			piece.render(m_gfx.get());
		}

		m_gfx->display();
	}

	for (auto& p : m_pieces)
	{
		SDL_DestroyTexture(p.tex());
	}

	m_gfx.reset();
}


void chess::draw_board()
{
	bool draw_colored = true;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			draw_colored = !draw_colored;

			if (x == 0)
			{
				if (y % 2 == 0)
					draw_colored = true;
				else
					draw_colored = false;
			}

			if (draw_colored)
				m_gfx->color({ 128, 0, 0 });
			else
				m_gfx->color({ 245, 222, 179 });

			SDL_Rect rect{ x * 100 + 100, y * 100 + 100, 100, 100 };
			SDL_RenderFillRect(m_gfx->rend(), &rect);
		}
	}

	m_gfx->color({ 0, 0, 0 });
}


void chess::handle_mouse(int px, int py, bool mouse_down)
{
	if (mouse_down)
	{
		if (!m_selected_piece)
		{
			for (auto& p : m_pieces)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				if (p.contains(x, y))
				{
					m_selected_piece = &p;
					m_valid_moves = p.get_valid_moves(m_pieces); 
					m_selected_piece_orig = { p.x(), p.y() };
					break;
				}
			}
		}
	}
	else
	{
		if (m_selected_piece)
		{
			if (valid_move())
			{
				m_selected_piece->move(
					m_selected_piece->cx() * 100 + 100 - m_selected_piece->rect_x(),
					m_selected_piece->cy() * 100 + 100 - m_selected_piece->rect_y()
				);
			}
			else
			{
				m_selected_piece->move(
					m_selected_piece_orig.x * 100 + 100 - m_selected_piece->rect_x(),
					m_selected_piece_orig.y * 100 + 100 - m_selected_piece->rect_y()
				);
			}

			m_valid_moves.clear();
			m_selected_piece = nullptr;
		}
	}
}


bool chess::valid_move()
{
	bool valid = false;

	for (auto& p : m_valid_moves)
	{
		if (m_selected_piece->cx() == p.x && m_selected_piece->cy() == p.y)
			valid = true;
	}

	return valid;
}