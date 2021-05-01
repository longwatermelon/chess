#pragma once
#include "graphics.h"
#include <memory>


namespace chess
{
	inline std::unique_ptr<Graphics> gfx;

	void init();

	void mainloop();
}