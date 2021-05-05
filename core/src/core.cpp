#include "../include/core.h"
#include <iostream>
#include <sstream>
#include <algorithm>


void core::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    m_gfx = std::make_unique<Graphics>("chess");

    m_font = TTF_OpenFont("assets/OpenSans-Regular.ttf", 100);
}


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
                m_gfx->color({ 245, 222, 179 });
            else
                m_gfx->color({ 128, 0, 0 });

            SDL_Rect rect{ x * 100 + 100, y * 100 + 100, 100, 100 };
            SDL_RenderFillRect(m_gfx->rend(), &rect);
        }
    }

    m_gfx->color({ 0, 0, 0 });
}


void core::handle_mouse(int px, int py, bool mouse_down, bool flipped)
{
    if (mouse_down)
    {
        if (!m_selected_piece)
        {
            for (auto& p : m_pieces)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (flipped)
                {
                    SDL_Point tmp = utils::flip_coords({ x, y });
                    x = tmp.x;
                    y = tmp.y;
                }

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
                multiplayer::m_newest_change = "type=new-move&";
                multiplayer::m_newest_change += "px=" + std::to_string(m_selected_piece_grid_orig.x) + "&py=" + std::to_string(m_selected_piece_grid_orig.y) + "&";

                m_selected_piece->grid_move_to(m_selected_piece->grid_cx(), m_selected_piece->grid_cy());

                Piece* piece = utils::piece_at(m_pieces, m_selected_piece->grid_x(), m_selected_piece->grid_y(), m_selected_piece);

                if (piece)
                {
                    utils::eat_piece(m_pieces, piece);
                }

                m_turn = (m_turn == Color::BLACK ? Color::WHITE : Color::BLACK);

                multiplayer::m_newest_change += "x=" + std::to_string(m_selected_piece->grid_x()) + "&y=" + std::to_string(m_selected_piece->grid_y());
            }
            else
            {
                m_selected_piece->grid_move_to(m_selected_piece_grid_orig.x, m_selected_piece_grid_orig.y);
            }


            m_valid_moves.clear();
            m_selected_piece = nullptr;

            check_kings();
        }
    }
}


void core::piece_follow_cursor(int px, int py, bool flipped)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    if (flipped)
    {
        SDL_Point tmp = utils::flip_coords({ x, y });
        x = tmp.x;
        y = tmp.y;
    }

    if (m_selected_piece)
    {
        m_selected_piece->move(x - px, y - py);
    }
}


void core::new_piece(PieceType type, Color color, int gridx, int gridy)
{
    m_pieces.emplace_back(std::make_unique<Piece>(type, color, gridx, gridy, m_gfx.get()));
}


Piece* core::piece_at(int x, int y)
{
    return utils::piece_at(m_pieces, x, y);
}


void core::remove_piece(Piece* piece)
{
    utils::eat_piece(m_pieces, piece);
}


void core::check_kings()
{
    m_white_check = utils::check(m_pieces, utils::get_king(m_pieces, Color::WHITE));
    m_black_check = utils::check(m_pieces, utils::get_king(m_pieces, Color::BLACK));

    m_white_checkmate = utils::checkmate(m_pieces, utils::get_king(m_pieces, Color::WHITE));
    m_black_checkmate = utils::checkmate(m_pieces, utils::get_king(m_pieces, Color::BLACK));
}


void core::clear_and_draw(bool flipped)
{
    m_gfx->clear();

    draw_board();

    m_gfx->color({ 0, 255, 0, 80 });

    if (m_selected_piece)
    {
        for (auto& p : m_valid_moves)
        {
            SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_BLEND);

            SDL_Point tmp;

            if (flipped)
                tmp = utils::flip_coords({ p.x * 100 + 200, p.y * 100 + 200 });
            else
                tmp = { p.x * 100 + 100, p.y * 100 + 100 };

            SDL_Rect rect{ tmp.x, tmp.y, 100, 100 };
            SDL_RenderFillRect(m_gfx->rend(), &rect);

            SDL_SetRenderDrawBlendMode(m_gfx->rend(), SDL_BLENDMODE_NONE);
        }
    }

    m_gfx->color({ 0, 0, 0, 255 });

    for (auto& piece : m_pieces)
    {
        if (piece.get() == m_selected_piece)
            continue; // selected piece will always be on the top layer if drawn last

        piece->render(m_gfx.get(), flipped);
    }

    if (m_selected_piece)
        m_selected_piece->render(m_gfx.get(), flipped);

    if (m_white_check && !m_white_checkmate)
        utils::draw_text(m_gfx.get(), m_font, "white is in check", 400, 950);

    if (m_white_checkmate)
        utils::draw_text(m_gfx.get(), m_font, "white is in checkmate", 340, 950);

    if (m_black_check && !m_black_checkmate)
        utils::draw_text(m_gfx.get(), m_font, "black is in check", 400, 50);

    if (m_black_checkmate)
        utils::draw_text(m_gfx.get(), m_font, "black is in checkmate", 340, 50);

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


std::string core::multiplayer::get_new_changes()
{
    std::string tmp = m_newest_change;
    m_newest_change.clear();

    return tmp;
}


std::string core::multiplayer::get_elem_from_string(const std::string& data, const std::string& elem)
{
    std::vector<std::string> data_list;

    std::stringstream ss(data);
    std::string pair;
    while (std::getline(ss, pair, '&')) data_list.emplace_back(pair);

    for (auto& str : data_list)
    {
        std::stringstream tmp(str);
        std::string key;
        std::getline(tmp, key, '=');

        std::string value;
        std::getline(tmp, value);

        if (key == elem)
            return value;
    }

    return "";
}