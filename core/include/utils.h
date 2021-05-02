#pragma once
#include "piece.h"


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