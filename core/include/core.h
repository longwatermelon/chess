#pragma once
#include "piece.h"
#include <memory>
#include <vector>


class ScopedMove
{
public:
    ScopedMove(Piece* piece, int gridx, int gridy);

    ~ScopedMove();

private:
    Piece* m_piece;
    SDL_Point m_orig_grid_point;
};


namespace core
{
    inline std::unique_ptr<Graphics> m_gfx{ std::make_unique<Graphics>("chess") };
    inline std::vector<Piece> m_pieces;

    inline Piece* m_selected_piece{ nullptr };
    inline SDL_Point m_selected_piece_grid_orig{ 0, 0 };

    inline std::vector<SDL_Point> m_valid_moves;

    inline Color m_turn{ Color::WHITE };

    inline Piece* b_king{ nullptr }, * w_king{ nullptr };

    void draw_board();

    void handle_mouse(int px, int py, bool mouse_down);

    bool valid_move(Piece& piece);

    Piece* piece_at(int x, int y, Piece* ignored = nullptr);
    void eat_piece(Piece& piece);

    void piece_follow_cursor(int px, int py);

    void clear_and_draw();

    void cleanup();

    void new_piece(PieceType type, Color color, int gridx, int gridy);

    void find_kings();

    bool check(Piece* king);
}