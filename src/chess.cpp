#include "../include/chess.h"


void chess::init()
{
	m_gfx = std::make_unique<Graphics>("chess");
	
	m_board += "........";
	m_board += "........";
	m_board += "........";
	m_board += "........";
	m_board += "........";
	m_board += "........";
	m_board += "........";
	m_board += "........";
}


void chess::mainloop()
{
	bool running = true;
	SDL_Event evt;

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT: running = false; break;
			}
		}

		m_gfx->clear();

		draw_board();

		m_gfx->display();
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
				m_gfx->color({ 255, 255, 255 });

			SDL_Rect rect{ x * 100 + 100, y * 100 + 100, 100, 100 };
			SDL_RenderFillRect(m_gfx->rend(), &rect);
		}
	}

	m_gfx->color({ 0, 0, 0 });
}