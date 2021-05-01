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

		return valid;
	}
	case PieceType::BISHOP:
		bool left = true, right = true;

		for (int i = y() - 1; i > -1; --i)
		{
			if (!occupied(x() - (y() - i), i, pieces) && left)
				valid.emplace_back(SDL_Point{ x() - (y() - i) , i });
			else
			{
				if (left && occupied(x() - (y() - i), i, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ x() - (y() - i), i });

				left = false;
			}

			if (!occupied(x() + (y() - i), i, pieces) && right)
				valid.emplace_back(SDL_Point{ x() + (y() - i) , i });
			else
			{
				if (right && occupied(x() + (y() - i), i, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ x() + (y() - i), i });

				right = false;
			}
		}

		left = true, right = true;

		for (int i = y() + 1; i < 8; ++i)
		{
			if (!occupied(x() - (y() - i), i, pieces) && left)
				valid.emplace_back(SDL_Point{ x() - (y() - i), i });
			else
			{
				if (left && occupied(x() - (y() - i), i, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ x() - (y() - i), i });
				left = false;
			}

			if (!occupied(x() + (y() - i), i, pieces) && right)
				valid.emplace_back(SDL_Point{ x() + (y() - i), i });
			else
			{
				if (right && occupied(x() + (y() - i), i, pieces)->color() != m_color)
					valid.emplace_back(SDL_Point{ x() + (y() - i), i });

				right = false;
			}
		}

		return valid;
	}

	return std::vector<SDL_Point>();
}


const Piece* Piece::occupied(int x, int y, const std::vector<Piece>& pieces)
{
	for (auto& p : pieces)
	{
		if (p.x() == x && p.y() == y)
			return &p;
	}

	return nullptr;
}