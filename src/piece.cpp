#include "../include/piece.h"
#include <string>


Piece::Piece(PieceType type, Color color, int x, int y, Graphics* gfx)
	: m_type(type), m_color(color)
{
	std::string fp = (m_color == Color::BLACK ? "assets/b-" : "assets/w-");

	switch (m_type)
	{
	case PieceType::BISHOP: fp += "bishop"; break;
	case PieceType::KING: fp += "king"; break;
	case PieceType::QUEEN: fp += "queen"; break;
	case PieceType::KNIGHT: fp += "knight"; break;
	case PieceType::PAWN: fp += "pawn"; break;
	case PieceType::ROOK: fp += "rook"; break;
	}

	fp += ".png";

	// some wasted memory since i could just reuse already created textures
	// but this is more convenient to code
	m_texture = gfx->load_texture(fp.c_str());

	m_rect.x = 100 + x * 100;
	m_rect.y = 100 + y * 100;
	m_rect.w = 100;
	m_rect.h = 100;
}


void Piece::render(Graphics* gfx)
{
	SDL_RenderCopy(gfx->rend(), m_texture, NULL, &m_rect);
}


bool Piece::contains(int x, int y)
{
	return m_rect.x <= x && m_rect.x + m_rect.w >= x
		&& m_rect.y <= y && m_rect.y + m_rect.h >= y;
}


void Piece::move(int x, int y)
{
	m_rect.x += x;
	m_rect.y += y;
}


std::vector<SDL_Point> Piece::get_valid_moves(const std::vector<Piece>& pieces)
{
	std::vector<SDL_Point> valid;

	switch (m_type)
	{
	case PieceType::PAWN:
	{
		int dir = (m_color == Color::BLACK ? -1 : 1);

		if (!occupied(x(), y() - 1 * dir, pieces))
			valid.emplace_back(SDL_Point{ x(), y() - 1 * dir });

		if ((m_color == Color::BLACK && y() == 1) || (m_color == Color::WHITE && y() == 6))
			if (!occupied(x(), y() - 2 * dir, pieces))
				valid.emplace_back(SDL_Point{ x(), y() - 2 * dir });

		for (int i = -1; i <= 1; i += 2)
		{
			const Piece* p = occupied(x() + i, y() - 1 * dir, pieces);
			
			if (p)
			{
				valid.emplace_back(SDL_Point{ x() + i, y() - 1 * dir });
			}
		}

		return valid;
	}
	case PieceType::BISHOP:
	{
		scan(1, 1, valid, pieces);
		scan(-1, -1, valid, pieces);
		scan(1, -1, valid, pieces);
		scan(-1, 1, valid, pieces);

		return valid;
	}
	case PieceType::ROOK:
	{
		scan(1, 0, valid, pieces);
		scan(-1, 0, valid, pieces);
		scan(0, 1, valid, pieces);
		scan(0, -1, valid, pieces);

		return valid;
	}
	case PieceType::KNIGHT:
	{
		for (int i = y() - 2; i <= y() + 2; ++i)
		{
			for (int j = x() - 2; j <= x() + 2; ++j)
			{
				if (abs((j - x()) * (i - y())) == 2)
				{
					const Piece* piece = occupied(j, i, pieces);

					if (piece && piece->color() == m_color)
						continue;

					if (j >= 0 && j < 8 && i >= 0 && i < 8)
						valid.emplace_back(SDL_Point{ j, i });
				}
			}
		}

		return valid;
	}
	case PieceType::KING:
	{
		for (int i = y() - 1; i <= y() + 1; ++i)
		{
			for (int j = x() - 1; j <= x() + 1; ++j)
			{
				const Piece* piece = occupied(j, i, pieces);

				if (piece && piece->color() == m_color)
					continue;

				if (x() == j && y() == i)
					continue;

				if (j >= 0 && j <= 7 && i >= 0 && i <= 7)
					valid.emplace_back(SDL_Point{ j, i });
			}
		}

		return valid;
	}
	case PieceType::QUEEN:
	{
		scan(1, 1, valid, pieces);
		scan(-1, -1, valid, pieces);
		scan(1, -1, valid, pieces);
		scan(-1, 1, valid, pieces);

		scan(1, 0, valid, pieces);
		scan(-1, 0, valid, pieces);
		scan(0, 1, valid, pieces);
		scan(0, -1, valid, pieces);

		return valid;
	}
	}

	return std::vector<SDL_Point>();
}


const Piece* Piece::occupied(int x, int y, const std::vector<Piece>& pieces, Piece* ignored)
{
	for (auto& p : pieces)
	{
		if (&p == ignored)
			continue;

		if (p.x() == x && p.y() == y)
			return &p;
	}

	return nullptr;
}


void Piece::scan(int xdir, int ydir, std::vector<SDL_Point>& valid, const std::vector<Piece>& pieces)
{
	bool blocked = false;

	if (ydir != 0 && xdir != 0)
	{
		int upper = (ydir == 1 ? 8 : -1);

		for (int i = y() + ydir; (upper < 0 ? i > upper : i < upper); i += ydir)
		{
			int current_x = x() + xdir * abs(y() - i);
			int current_y = i;

			if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
				continue;

			if (!occupied(current_x, current_y, pieces) && !blocked)
			{
				valid.emplace_back(SDL_Point{ current_x, current_y });
			}
			else
			{
				if (!blocked && occupied(current_x, current_y, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ current_x, current_y });

				blocked = true;
			}
		}
	}
	else
	{
		int begin;
		int dir;
		bool static_x = false;

		if (ydir == 0)
		{
			begin = x() + xdir;
			dir = xdir;
		}
		else
		{
			begin = y() + ydir;
			dir = ydir;
			static_x = true;
		}

		for (int i = begin; (dir == 1 ? i < 8 : i > -1); i += dir)
		{
			int ox, oy;
			if (static_x)
			{
				oy = i;
				ox = x();
			}
			else
			{
				oy = y();
				ox = i;
			}

			if (!occupied(ox, oy, pieces) && !blocked)
				valid.emplace_back(SDL_Point{ ox, oy });
			else
			{
				if (!blocked && occupied(ox, oy, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ ox, oy });

				blocked = true;
			}
		}
	}
}