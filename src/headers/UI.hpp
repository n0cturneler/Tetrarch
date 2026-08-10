#pragma once

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Board.hpp"

#include <raylib.h>

#include <chrono>
#include <string_view>

namespace UI
{	
	extern Font mainFont;
	extern Font mainFontBig;
	extern Font mainFontSmall;

	extern float fontSpacing;

	extern int screenWidth;
	extern int screenHeight;

	void initialize();

	Vector2 getCenteredTextPos(const Rectangle& box, float fontSize, const char* text);

	void FPS();
	void lockDelay(const piece::Piece& activePiece, const board::Board& curBoard);
	void drawBag(const bag::Bag& currentBag, const bag::Bag& nextBag, const board::Board& curBoard, int bagDisplayCount);
}
