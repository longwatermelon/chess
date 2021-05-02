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

                if (p.contains(x, y) && p.color() == m_turn)
                {
                    m_selected_piece = &p;
                    m_selected_piece_orig = { p.x(), p.y() };
                    m_valid_moves = m_selected_piece->get_valid_moves(m_pieces);
                    break;
                }
            }
        }
    }
    else
    {
        if (m_selected_piece)
        {
            if (valid_move(*m_selected_piece))
            {
                m_selected_piece->move_to(m_selected_piece->cx(), m_selected_piece->cy());

                Piece* piece = piece_at(m_selected_piece->x(), m_selected_piece->y(), m_selected_piece);

                if (piece)
                {
                    eat_piece(*piece);
                }

                m_turn = (m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);
            }
            else
            {
                m_selected_piece->move_to(m_selected_piece_orig.x, m_selected_piece_orig.y);
            }

            m_valid_moves.clear();
            m_selected_piece = nullptr;

            find_kings();

            if (check(w_king))
                std::cout << "white is in check\n";

            if (check(b_king))
                std::cout << "black is in check\n";
        }
    }
}


bool core::valid_move(Piece& piece)
{
    bool valid = false;

    for (auto& p : m_valid_moves)
    {
        if (m_selected_piece->cx() == p.x && m_selected_piece->cy() == p.y)
            valid = true;
    }

    return valid;
}


Piece* core::piece_at(int x, int y, Piece* ignored)
{
    for (auto& p : m_pieces)
    {
        if (&p == ignored)
            continue;

        if (p.x() == x && p.y() == y)
            return &p;
    }

    return nullptr;
}


void core::eat_piece(Piece& piece)
{
    for (int i = 0; i < m_pieces.size(); ++i)
    {
        if (&m_pieces[i] == &piece)
        {
            m_pieces.erase(m_pieces.begin() + i);
            break;
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
        if (&piece == m_selected_piece)
            continue; // selected piece will always be on the top layer if drawn last

        piece.render(m_gfx.get());
    }

    if (m_selected_piece)
        m_selected_piece->render(m_gfx.get());

    m_gfx->display();
}


void core::cleanup()
{
    for (auto& p : m_pieces)
    {
        SDL_DestroyTexture(p.tex());
    }

    m_gfx.reset();
}


void core::new_piece(PieceType type, Color color, int x, int y)
{
    m_pieces.emplace_back(Piece(type, color, x, y, m_gfx.get()));
}


void core::find_kings()
{
    for (auto& p : m_pieces)
    {
        if (p.type() == PieceType::KING)
        {
            switch (p.color())
            {
            case Color::BLACK: b_king = &p; break;
            case Color::WHITE: w_king = &p; break;
            }
        }
    }
}


bool core::check(Piece* king)
{
    bool in_check = false;

    for (auto& piece : m_pieces)
    {
        if (piece.color() == king->color())
            continue;

        for (auto& m : piece.get_valid_moves(m_pieces))
        {
            if (m.x == king->x() && m.y == king->y())
                in_check = true;
        }
    }

    return in_check;
}