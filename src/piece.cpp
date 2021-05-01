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