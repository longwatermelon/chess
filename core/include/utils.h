#pragma once
#include "piece.h"


namespace utils
{
    class ScopedMove
    {
    public:
        ScopedMove(Piece* piece, int gridx, int gridy, SDL_Point orig_point);

        ~ScopedMove();

    private:
        Piece* m_piece;
        SDL_Point m_orig_grid_point;
    };


    class ScopedErase
    {
    public:
        ScopedErase(Piece* piece, std::vector<std::unique_ptr<Piece>>& pieces);

        ~ScopedErase();

    private:
        std::unique_ptr<Piece> m_piece;
        std::vector<std::unique_ptr<Piece>>& m_pieces;
    };


    std::vector<SDL_Point> get_real_valid_moves(std::vector<std::unique_ptr<Piece>>& pieces, Piece* piece, SDL_Point orig);
    bool is_valid_move(std::vector<std::unique_ptr<Piece>>& pieces, std::vector<SDL_Point>& valid, Piece* piece, int grid_x, int grid_y);

    Piece* get_king(std::vector<std::unique_ptr<Piece>>& pieces, Color color);

    bool check(std::vector<std::unique_ptr<Piece>>& pieces, Piece* king);
    bool checkmate(std::vector<std::unique_ptr<Piece>>& pieces, Piece* king);

    Piece* piece_at(std::vector<std::unique_ptr<Piece>>& pieces, int grid_x, int grid_y, Piece* ignored = nullptr);
    void eat_piece(std::vector<std::unique_ptr<Piece>>& pieces, Piece* piece);
}