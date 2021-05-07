#pragma once
#include "../src/piece.h"
#include "../src/utils.h"
#include <memory>
#include <vector>
#include <string>


namespace core
{
    inline std::unique_ptr<Graphics> m_gfx;
    inline std::vector<std::unique_ptr<Piece>> m_pieces;

    inline Piece* m_selected_piece{ nullptr };
    inline SDL_Point m_selected_piece_grid_orig{ 0, 0 };

    inline std::vector<SDL_Point> m_valid_moves;

    inline Color m_turn{ Color::WHITE };

    inline TTF_Font* m_font = nullptr;

    inline bool m_white_check = false;
    inline bool m_white_checkmate = false;
    inline bool m_black_check = false;
    inline bool m_black_checkmate = false;

    void init();

    void draw_board();

    void handle_mouse(int px, int py, bool mouse_down, bool flipped);

    void piece_follow_cursor(int px, int py, bool flipped);

    void new_piece(PieceType type, Color color, int gridx, int gridy);
    Piece* piece_at(int x, int y);
    void remove_piece(Piece* piece);

    void check_kings();

    void clear_and_draw(bool flipped);

    void cleanup();


    namespace multiplayer
    {
        inline std::string m_newest_change;

        std::string get_new_changes();

        std::string get_elem_from_string(const std::string& data, const std::string& elem);
    }
}
