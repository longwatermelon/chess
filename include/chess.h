#pragma once
#include "graphics.h"
#include "piece.h"
#include <memory>
#include <string>
#include <vector>


namespace chess
{
	inline std::unique_ptr<Graphics> m_gfx;
	inline std::vector<Piece> m_pieces;

	void init();

	void mainloop();

	void draw_board();
}