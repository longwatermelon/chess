#include "../include/utils.h"


ScopedMove::ScopedMove(Piece* piece, int gridx, int gridy, SDL_Point orig_point)
    : m_piece(piece), m_orig_grid_point(orig_point)
{
    piece->grid_move_to(gridx, gridy);
}


ScopedMove::~ScopedMove()
{
    m_piece->grid_move_to(m_orig_grid_point.x, m_orig_grid_point.y);
}