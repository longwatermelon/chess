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


ScopedErase::ScopedErase(Piece* piece, std::vector<std::unique_ptr<Piece>>& pieces)
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


ScopedErase::~ScopedErase()
{
    m_pieces.emplace_back(std::move(m_piece));
}