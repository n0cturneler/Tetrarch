#include "UI.hpp"

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Options.hpp"
using namespace options;

#include <raygui.h>
#include <raylib.h>

#include <chrono>
#include <format>
#include <string>
#include <string_view>
#include <iostream>

namespace UI
{
	Font mainFont;
	Font mainFontBig;
	Font mainFontSmall;

	void UI::initialize()
	{
		mainFont = LoadFont("assets/fonts/Space_Grotesk.ttf");
		mainFontBig = LoadFontEx("assets/fonts/Space_Grotesk.ttf", 48, nullptr, 0);
		mainFontSmall = LoadFontEx("assets/fonts/Space_Grotesk.ttf", 16, nullptr, 0);

		GuiSetFont(mainFont);
		GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
	}

	void UI::FPS()
	{
		
	}

	void UI::lockDelay(const piece::Piece& activePiece, const board::Board& curBoard)
	{
		
	}

	void UI::drawBag(bag::Bag& currentBag, bag::Bag& nextBag)
	{
		
	}


}
