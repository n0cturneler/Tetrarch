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
	extern Font	mainFontSmall;

	void initialize();

	void FPS();
	void lockDelay(const piece::Piece& activePiece, const board::Board& curBoard);
	void drawBag(bag::Bag& currentBag, bag::Bag& nextBag);
}
