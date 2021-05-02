#pragma once
#include "piece.h"
#include <memory>
#include <vector>


namespace core
{
    inline std::unique_ptr<Graphics> m_gfx{ std::make_unique<Graphics>("chess") };
    inline std::vector<Piece> m_pieces;

    inline Piece* m_selected_piece{ nullptr };
    inline SDL_Point m_selected_piece_orig{ 0, 0 };

    inline std::vector<SDL_Point> m_valid_moves;

    inline Color m_turn{ Color::WHITE };

    void draw_board();

    void handle_mouse(int px, int py, bool mouse_down);

    bool valid_move(Piece& piece);

    Piece* piece_at(int x, int y, Piece* ignored = nullptr);
    void eat_piece(Piece& piece);

    void piece_follow_cursor(int px, int py);

    void clear_and_draw();

    void cleanup();

    void new_piece(PieceType type, Color color, int x, int y);
}