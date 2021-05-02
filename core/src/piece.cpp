#include "../include/piece.h"
#include "../include/utils.h"
#include <string>


Piece::Piece(PieceType type, Color color, int x, int y, Graphics* gfx)
    : m_type(type), m_color(color)
{
    std::string fp = (m_color == Color::BLACK ? "assets/b-" : "assets/w-");

    switch (m_type)
    {
    case PieceType::BISHOP: fp += "bishop"; break;
    case PieceType::KING: fp += "king"; break;
    case PieceType::QUEEN: fp += "queen"; break;
    case PieceType::KNIGHT: fp += "knight"; break;
    case PieceType::PAWN: fp += "pawn"; break;
    case PieceType::ROOK: fp += "rook"; break;
    }

    fp += ".png";

    // some wasted memory since i could just reuse already created textures
    // but this is more convenient to code
    m_texture = gfx->load_texture(fp.c_str());

    m_rect.x = 100 + x * 100;
    m_rect.y = 100 + y * 100;
    m_rect.w = 100;
    m_rect.h = 100;
}


void Piece::render(Graphics* gfx)
{
    SDL_RenderCopy(gfx->rend(), m_texture, NULL, &m_rect);
}


bool Piece::contains(int x, int y)
{
    return m_rect.x <= x && m_rect.x + m_rect.w >= x
        && m_rect.y <= y && m_rect.y + m_rect.h >= y;
}


void Piece::move(int x, int y)
{
    m_rect.x += x;
    m_rect.y += y;
}


void Piece::grid_move_to(int gx, int gy)
{
    move(
        gx * 100 + 100 - m_rect.x,
        gy * 100 + 100 - m_rect.y
    );
}


std::vector<SDL_Point> Piece::get_valid_moves(std::vector<std::unique_ptr<Piece>>& pieces)
{
    std::vector<SDL_Point> valid;

    switch (m_type)
    {
    case PieceType::PAWN:
    {
        int dir = (m_color == Color::BLACK ? -1 : 1);

        if (!utils::piece_at(pieces, x(), y() - 1 * dir))
            valid.emplace_back(SDL_Point{ x(), y() - 1 * dir });

        if ((m_color == Color::BLACK && y() == 1) || (m_color == Color::WHITE && y() == 6))
            if (!utils::piece_at(pieces, x(), y() - 2 * dir))
                valid.emplace_back(SDL_Point{ x(), y() - 2 * dir });

        for (int i = -1; i <= 1; i += 2)
        {
            Piece* p = utils::piece_at(pieces, x() + i, y() - 1 * dir);
            
            if (p && p->color() != m_color)
            {
                valid.emplace_back(SDL_Point{ x() + i, y() - 1 * dir });
            }
        }

        return valid;
    }
    case PieceType::BISHOP:
    {
        scan(1, 1, valid, pieces);
        scan(-1, -1, valid, pieces);
        scan(1, -1, valid, pieces);
        scan(-1, 1, valid, pieces);

        return valid;
    }
    case PieceType::ROOK:
    {
        scan(1, 0, valid, pieces);
        scan(-1, 0, valid, pieces);
        scan(0, 1, valid, pieces);
        scan(0, -1, valid, pieces);

        return valid;
    }
    case PieceType::KNIGHT:
    {
        for (int i = y() - 2; i <= y() + 2; ++i)
        {
            for (int j = x() - 2; j <= x() + 2; ++j)
            {
                if (abs((j - x()) * (i - y())) == 2)
                {
                    Piece* piece = utils::piece_at(pieces, j, i);

                    if (piece && piece->color() == m_color)
                        continue;

                    if (j >= 0 && j < 8 && i >= 0 && i < 8)
                        valid.emplace_back(SDL_Point{ j, i });
                }
            }
        }

        return valid;
    }
    case PieceType::KING:
    {
        for (int i = y() - 1; i <= y() + 1; ++i)
        {
            for (int j = x() - 1; j <= x() + 1; ++j)
            {
                Piece* piece = utils::piece_at(pieces, j, i);

                if (piece && piece->color() == m_color)
                    continue;

                if (x() == j && y() == i)
                    continue;

                if (j >= 0 && j <= 7 && i >= 0 && i <= 7)
                    valid.emplace_back(SDL_Point{ j, i });
            }
        }

        return valid;
    }
    case PieceType::QUEEN:
    {
        scan(1, 1, valid, pieces);
        scan(-1, -1, valid, pieces);
        scan(1, -1, valid, pieces);
        scan(-1, 1, valid, pieces);

        scan(1, 0, valid, pieces);
        scan(-1, 0, valid, pieces);
        scan(0, 1, valid, pieces);
        scan(0, -1, valid, pieces);

        return valid;
    }
    }

    return std::vector<SDL_Point>();
}


void Piece::scan(int xdir, int ydir, std::vector<SDL_Point>& valid, std::vector<std::unique_ptr<Piece>>& pieces)
{
    bool blocked = false;

    if (ydir != 0 && xdir != 0)
    {
        int upper = (ydir == 1 ? 8 : -1);

        for (int i = y() + ydir; (upper < 0 ? i > upper : i < upper); i += ydir)
        {
            int current_x = x() + xdir * abs(y() - i);
            int current_y = i;

            if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                continue;

            if (!utils::piece_at(pieces, current_x, current_y) && !blocked)
            {
                valid.emplace_back(SDL_Point{ current_x, current_y });
            }
            else
            {
                if (!blocked && utils::piece_at(pieces, current_x, current_y)->color() != m_color)
                    valid.emplace_back(SDL_Point{ current_x, current_y });

                blocked = true;
            }
        }
    }
    else
    {
        int begin;
        int dir;
        bool static_x = false;

        if (ydir == 0)
        {
            begin = x() + xdir;
            dir = xdir;
        }
        else
        {
            begin = y() + ydir;
            dir = ydir;
            static_x = true;
        }

        for (int i = begin; (dir == 1 ? i < 8 : i > -1); i += dir)
        {
            int ox, oy;
            if (static_x)
            {
                oy = i;
                ox = x();
            }
            else
            {
                oy = y();
                ox = i;
            }

            if (!utils::piece_at(pieces, ox, oy) && !blocked)
                valid.emplace_back(SDL_Point{ ox, oy });
            else
            {
                if (!blocked && utils::piece_at(pieces, ox, oy)->color() != m_color)
                    valid.emplace_back(SDL_Point{ ox, oy });

                blocked = true;
            }
        }
    }
}