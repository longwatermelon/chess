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
	inline Piece* m_selected_piece;

	void init();

	void mainloop();

	void draw_board();

	void handle_mouse(int px, int py, bool mouse_down);
}