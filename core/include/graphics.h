#pragma once
#include <SDL.h>
#include <SDL_image.h>


class Graphics
{
public:
    Graphics(const char* title);

    ~Graphics();

    SDL_Texture* load_texture(const char* fp);

    SDL_Renderer* rend() { return m_rend; }
    SDL_Window* window() { return m_window; }

    void clear() const { SDL_RenderClear(m_rend); }
    void display() const { SDL_RenderPresent(m_rend); }
    void color(SDL_Color col) { SDL_SetRenderDrawColor(m_rend, col.r, col.g, col.b, col.a); }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_rend;
};