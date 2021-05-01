#include "../include/graphics.h"


Graphics::Graphics(const char* title)
{
	SDL_Init(SDL_INIT_VIDEO);
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
	m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);
	SDL_RenderClear(m_rend);
	SDL_RenderPresent(m_rend);
}


Graphics::~Graphics()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_rend);

	SDL_Quit();
}