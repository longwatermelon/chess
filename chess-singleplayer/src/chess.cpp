#include "../include/chess.h"
#include <string>


void chess::mainloop()
{
	for (int i = 0; i < 8; ++i)
	{
		core::new_piece(PieceType::PAWN, Color::BLACK, i, 1);
		core::new_piece(PieceType::PAWN, Color::WHITE, i, 6);
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

		core::piece_follow_cursor(prev_x, prev_y);

		SDL_GetMouseState(&prev_x, &prev_y);

		core::clear_and_draw();
	}

	core::cleanup();
}

#if 0
#include "../include/chess.h"
#include <iostream>


void chess::init()
{
	std::cout << "loading pieces\n";

	m_gfx = std::make_unique<Graphics>("chess");
	m_selected_piece = nullptr;
	m_turn = Color::WHITE;

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

		m_gfx->color({ 0, 255, 0, 80 });

		for (auto& p : m_valid_moves)
		{
			SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_BLEND);

			SDL_Rect rect{ p.x * 100 + 100, p.y * 100 + 100, 100, 100 };
			SDL_RenderFillRect(m_gfx->rend(), &rect);

			SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_NONE);
		}

		m_gfx->color({ 0, 0, 0, 255 });

		for (auto& piece : m_pieces)
		{
			if (&piece == m_selected_piece)
				continue;

			piece.render(m_gfx.get());
		}

		if (m_selected_piece)
			m_selected_piece->render(m_gfx.get());

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

				if (p.contains(x, y) && p.color() == m_turn)
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

				const Piece* piece = m_selected_piece->occupied(m_selected_piece->cx(), m_selected_piece->cy(), m_pieces, m_selected_piece);

				if (piece)
				{
					for (int i = 0; i < m_pieces.size(); ++i)
					{
						if (&m_pieces[i] == piece)
						{
							m_pieces.erase(m_pieces.begin() + i);
							break;
						}
					}
				}

				m_turn = (m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);
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
#endif