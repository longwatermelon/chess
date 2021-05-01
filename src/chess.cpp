#include "../include/chess.h"


void chess::init()
{
	gfx = std::make_unique<Graphics>("chess");
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

		gfx->clear();

		gfx->display();
	}

	gfx.reset();
}