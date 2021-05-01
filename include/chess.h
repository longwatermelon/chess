#pragma once
#include "graphics.h"
#include <memory>
#include <string>


namespace chess
{
	inline std::unique_ptr<Graphics> m_gfx;
	inline std::string m_board;

	void init();

	void mainloop();

	void draw_board();
}