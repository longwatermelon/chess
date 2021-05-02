#pragma once
#include "graphics.h"
#include <vector>
#include <memory>


enum class PieceType
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN
};

enum class Color
{
    BLACK,
    WHITE
};


class Piece
{
public:
    Piece(PieceType type, Color color, int x, int y, Graphics* gfx);

    void render(Graphics* gfx);

    bool contains(int x, int y);
    void move(int x, int y);
    void grid_move_to(int gx, int gy);

    std::vector<SDL_Point> get_valid_moves(std::vector<std::unique_ptr<Piece>>& pieces);

    void scan(int xdir, int ydir, std::vector<SDL_Point>& valid, std::vector<std::unique_ptr<Piece>>& pieces);

    PieceType type() const { return m_type; }
    Color color() const { return m_color; }

    int grid_x() const { return (m_rect.x - 100) / 100; }
    int grid_y() const { return (m_rect.y - 100) / 100; }

    int grid_cx() const { return (m_rect.x + 50 - 100) / 100; }
    int grid_cy() const { return (m_rect.y + 50 - 100) / 100; }

    int rect_x() const { return m_rect.x; }
    int rect_y() const { return m_rect.y; }

    SDL_Texture* tex() const { return m_texture; }

private:
    PieceType m_type;
    Color m_color;

    SDL_Rect m_rect;

    SDL_Texture* m_texture;
};