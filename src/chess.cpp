#include "../include/chess.h"


void chess::init()
{
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
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_down = false;
				handle_mouse(prev_x, prev_y, mouse_down);
				break;
			}
		}

		if (mouse_down) handle_mouse(prev_x, prev_y, mouse_down);

		SDL_GetMouseState(&prev_x, &prev_y);

		m_gfx->clear();

		draw_board();

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
					break;
				}
			}
		}
		else
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			m_selected_piece->move(x - px, y - py);
		}
	}
	else
	{
		m_selected_piece = nullptr;
	}
}