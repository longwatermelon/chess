#pragma once
#include "graphics.h"


enum class PieceType
{
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	KING,
	QUEEN
};

enum class Color
{
	BLACK,
	WHITE
};


class Piece
{
public:
	Piece(PieceType type, Color color, int x, int y, Graphics* gfx);

	void render(Graphics* gfx);

	PieceType type() const { return m_type; }
	Color color() const { return m_color; }

	int x() const { return (m_rect.x - 100) / 100; }
	int y() const { return (m_rect.y - 100) / 100; }

	SDL_Texture* tex() const { return m_texture; }

private:
	PieceType m_type;
	Color m_color;

	SDL_Rect m_rect;

	SDL_Texture* m_texture;
};