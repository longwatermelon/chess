#pragma once
#include "piece.h"
#include "utils.h"
#include <memory>
#include <vector>


namespace core
{
    inline std::unique_ptr<Graphics> m_gfx{ std::make_unique<Graphics>("chess") };
    inline std::vector<std::unique_ptr<Piece>> m_pieces;

    inline Piece* m_selected_piece{ nullptr };
    inline SDL_Point m_selected_piece_grid_orig{ 0, 0 };

    inline std::vector<SDL_Point> m_valid_moves;

    inline Color m_turn{ Color::WHITE };

    void draw_board();

    void handle_mouse(int px, int py, bool mouse_down);

    void piece_follow_cursor(int px, int py);
    void new_piece(PieceType type, Color color, int gridx, int gridy);

    void clear_and_draw();

    void cleanup();
}