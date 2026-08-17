#pragma once

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Board.hpp"

#include <raylib.h>

#include <chrono>
#include <string_view>

namespace UI
{	
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using MS = std::chrono::milliseconds;

	extern Font mainFont;
	extern Font mainFontBig;
	extern Font mainFontSmall;
	extern Font mainFontVeryBig;

	void initialize();

	Vector2 getCenteredTextPos(const Rectangle& box, float fontSize, const char* text);

	void FPS();

	void lockDelay(const piece::Piece& activePiece, const board::Board& curBoard);

	void bag();
	void bag3D(const board::Board& curBoard, int bagDisplayCount);

	void hold();
	void hold3D(const board::Board& curBoard);

	void attackBar();

	void pieceStats(const board::Board& curBoard);
	void attackStats(const board::Board& curBoard);
	void time(const board::Board& curBoard);
}
