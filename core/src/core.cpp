#include "../include/core.h"
#include <iostream>


void core::draw_board()
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


void core::handle_mouse(int px, int py, bool mouse_down)
{
    if (mouse_down)
    {
        if (!m_selected_piece)
        {
            for (auto& p : m_pieces)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (p->contains(x, y) && p->color() == m_turn)
                {
                    m_selected_piece = p.get();
                    m_selected_piece_grid_orig = { p->grid_x(), p->grid_y() };
                    m_valid_moves = utils::get_real_valid_moves(m_pieces, m_selected_piece, m_selected_piece_grid_orig);

                    break;
                }
            }
        }
    }
    else
    {
        if (m_selected_piece)
        {
            if (utils::is_valid_move(m_pieces, m_valid_moves, m_selected_piece, m_selected_piece->grid_cx(), m_selected_piece->grid_cy()))
            {
                m_selected_piece->grid_move_to(m_selected_piece->grid_cx(), m_selected_piece->grid_cy());

                Piece* piece = utils::piece_at(m_pieces, m_selected_piece->grid_x(), m_selected_piece->grid_y(), m_selected_piece);

                if (piece)
                {
                    utils::eat_piece(m_pieces, piece);
                }

                m_turn = (m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);
            }
            else
            {
                m_selected_piece->grid_move_to(m_selected_piece_grid_orig.x, m_selected_piece_grid_orig.y);
            }

            m_valid_moves.clear();
            m_selected_piece = nullptr;

            if (utils::check(m_pieces, utils::get_king(m_pieces, Color::WHITE)))
                std::cout << "white is in check\n";

            if (utils::check(m_pieces, utils::get_king(m_pieces, Color::BLACK)))
                std::cout << "black is in check\n";

            if (utils::checkmate(m_pieces, utils::get_king(m_pieces, Color::WHITE)))
                std::cout << "white is in checkmate, black wins\n";

            if (utils::checkmate(m_pieces, utils::get_king(m_pieces, Color::BLACK)))
                std::cout << "black is in checkmate, white wins\n";
        }
    }
}


void core::piece_follow_cursor(int px, int py)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (m_selected_piece)
    {
        m_selected_piece->move(x - px, y - py);
    }
}


void core::new_piece(PieceType type, Color color, int gridx, int gridy)
{
    m_pieces.emplace_back(std::make_unique<Piece>(type, color, gridx, gridy, m_gfx.get()));
}


void core::clear_and_draw()
{
    m_gfx->clear();

    draw_board();

    m_gfx->color({ 0, 255, 0, 80 });

    if (m_selected_piece)
    {
        for (auto& p : m_valid_moves)
        {
            SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_BLEND);

            SDL_Rect rect{ p.x * 100 + 100, p.y * 100 + 100, 100, 100 };
            SDL_RenderFillRect(m_gfx->rend(), &rect);

            SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_NONE);
        }
    }

    m_gfx->color({ 0, 0, 0, 255 });

    for (auto& piece : m_pieces)
    {
        if (piece.get() == m_selected_piece)
            continue; // selected piece will always be on the top layer if drawn last

        piece->render(m_gfx.get());
    }

    if (m_selected_piece)
        m_selected_piece->render(m_gfx.get());

    m_gfx->display();
}


void core::cleanup()
{
    for (auto& p : m_pieces)
    {
        SDL_DestroyTexture(p->tex());
    }

    m_gfx.reset();
}