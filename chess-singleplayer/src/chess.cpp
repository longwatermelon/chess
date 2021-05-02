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