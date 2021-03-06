#include "utils.h"


core::utils::ScopedMove::ScopedMove(Piece* piece, int gridx, int gridy, SDL_Point orig_point)
    : m_piece(piece), m_orig_grid_point(orig_point)
{
    piece->grid_move_to(gridx, gridy);
}


core::utils::ScopedMove::~ScopedMove()
{
    m_piece->grid_move_to(m_orig_grid_point.x, m_orig_grid_point.y);
}


core::utils::ScopedErase::ScopedErase(Piece* piece, std::vector<std::unique_ptr<Piece>>& pieces)
    : m_pieces(pieces)
{
    for (int i = 0; i < pieces.size(); ++i)
    {
        if (pieces[i].get() == piece)
        {
            m_piece = std::move(pieces[i]);
            pieces.erase(pieces.begin() + i);
            break;
        }
    }
}


core::utils::ScopedErase::~ScopedErase()
{
    m_pieces.emplace_back(std::move(m_piece));
}


std::vector<SDL_Point> core::utils::get_real_valid_moves(std::vector<std::unique_ptr<Piece>>& pieces, Piece* piece, SDL_Point orig)
{
    std::vector<SDL_Point> valid = piece->get_valid_moves(pieces);

    Piece* king = get_king(pieces, piece->color());

    for (int i = 0; i < valid.size(); ++i)
    {
        ScopedMove move(piece, valid[i].x, valid[i].y, orig);
        ScopedErase* erase = nullptr;

        if (piece_at(pieces, valid[i].x, valid[i].y, piece))
        {
            ScopedErase* tmp = new ScopedErase(piece_at(pieces, valid[i].x, valid[i].y, piece), pieces);
            erase = tmp;
        }

        if (check(pieces, king))
        {
            valid.erase(valid.begin() + i--);
        }

        delete erase;
    }

    return valid;
}


bool core::utils::is_valid_move(std::vector<std::unique_ptr<Piece>>& pieces, std::vector<SDL_Point>& valid, Piece* piece, int grid_x, int grid_y)
{
    bool is_valid = false;

    for (auto& p : valid)
    {
        if (piece->grid_cx() == p.x && piece->grid_cy() == p.y)
            is_valid = true;
    }

    return is_valid;
}


Piece* core::utils::get_king(std::vector<std::unique_ptr<Piece>>& pieces, Color color)
{
    for (auto& p : pieces)
    {
        if (p->type() == PieceType::KING && p->color() == color)
            return p.get();
    }

    return nullptr;
}


bool core::utils::check(std::vector<std::unique_ptr<Piece>>& pieces, Piece* king)
{
    bool in_check = false;

    for (auto& p : pieces)
    {
        if (p->color() == king->color())
            continue;

        for (auto& m : p->get_valid_moves(pieces))
        {
            if (m.x == king->grid_x() && m.y == king->grid_y())
                in_check = true;
        }
    }

    return in_check;
}


bool core::utils::checkmate(std::vector<std::unique_ptr<Piece>>& pieces, Piece* king)
{
    bool is_checkmate = true;

    for (auto& p : pieces)
    {
        if (p->color() != king->color())
            continue;

        std::vector<SDL_Point> valid = get_real_valid_moves(pieces, p.get(), { p->grid_x(), p->grid_y() });

        if (valid.size() > 0)
            is_checkmate = false;
    }

    return is_checkmate;
}


Piece* core::utils::piece_at(std::vector<std::unique_ptr<Piece>>& pieces, int grid_x, int grid_y, Piece* ignored)
{
    for (auto& p : pieces)
    {
        if (p.get() == ignored)
            continue;

        if (p->grid_x() == grid_x && p->grid_y() == grid_y)
            return p.get();
    }

    return nullptr;
}


void core::utils::eat_piece(std::vector<std::unique_ptr<Piece>>& pieces, Piece* piece)
{
    for (int i = 0; i < pieces.size(); ++i)
    {
        if (pieces[i].get() == piece)
        {
            pieces.erase(pieces.begin() + i);
            break;
        }
    }
}


void core::utils::draw_text(Graphics* gfx, TTF_Font* font, const char* text, int x, int y)
{
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, { 255, 255, 255 });
    SDL_Texture* tex = SDL_CreateTextureFromSurface(gfx->rend(), surf);

    SDL_Rect tmp = { x, y, strlen(text) * 15, 30 };
    SDL_RenderCopy(gfx->rend(), tex, 0, &tmp);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}


SDL_Point core::utils::flip_coords(SDL_Point p)
{
    return { 1000 - p.x, 1000 - p.y };
}
