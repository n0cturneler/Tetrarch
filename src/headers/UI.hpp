#pragma once

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Board.hpp"

#include <raylib.h>

#include <imgui.h>

#include <chrono>
#include <string_view>

namespace UI
{	
	inline ImFont* mainFont = nullptr;
	inline ImFont* mainFontBig = nullptr;
	inline ImFont* mainFontSmall = nullptr;
	inline ImFont* tetrisFontMedium = nullptr;

	void initialize();

	void FPS();
	void lockDelay(const piece::Piece& activePiece, const board::Board& curBoard);
	void drawBag(bag::Bag& currentBag, bag::Bag& nextBag);

	std::string_view textToTetrisFont(std::string_view text);

}
