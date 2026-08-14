#include "UI.hpp"

#include "Pieces.hpp"
#include "Bag.hpp"
#include "Options.hpp"
using namespace options;

#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#include <chrono>
#include <format>
#include <string>
#include <string_view>
#include <iostream>
#include <cstdint>
#include <chrono>

namespace UI
{
	Font mainFont;
	Font mainFontBig;
	Font mainFontSmall;
	Font mainFontVeryBig;

	float fontSpacing{2.0f};
	float borderThickness{5.0f};

	int screenWidth{};
	int screenHeight{};

	void initialize()
	{
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();

		mainFont = LoadFontEx("assets/fonts/blockletter.regular.otf", 24, nullptr, 0);
		SetTextureFilter(mainFont.texture, TEXTURE_FILTER_BILINEAR);

		mainFontBig = LoadFontEx("assets/fonts/blockletter.regular.otf", 32, nullptr, 0);
		SetTextureFilter(mainFontBig.texture, TEXTURE_FILTER_BILINEAR);

		mainFontVeryBig = LoadFontEx("assets/fonts/blockletter.regular.otf", 64, nullptr, 0);
		SetTextureFilter(mainFontBig.texture, TEXTURE_FILTER_BILINEAR);

		mainFontSmall = LoadFontEx("assets/fonts/blockletter.regular.otf", 16, nullptr, 0);
		SetTextureFilter(mainFontSmall.texture, TEXTURE_FILTER_BILINEAR);

		GuiSetFont(mainFont);
		GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

		GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(colors::textLight));

		GuiSetStyle(PROGRESSBAR, BORDER_WIDTH, 2);
		GuiSetStyle(PROGRESSBAR, PROGRESS_PADDING, 2);

		GuiSetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL, ColorToInt(colors::textBlueGray));
		GuiSetStyle(PROGRESSBAR, BORDER_COLOR_FOCUSED, ColorToInt(colors::textBlueGray));
		GuiSetStyle(PROGRESSBAR, BORDER_COLOR_PRESSED, ColorToInt(colors::textBlueGray));

		GuiSetStyle(PROGRESSBAR, BASE_COLOR_NORMAL, ColorToInt(colors::textBlueGray));
		GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED, ColorToInt(colors::uiBGLight));
		GuiSetStyle(PROGRESSBAR, TEXT_COLOR_NORMAL, ColorToInt(colors::textBlueGray));
	}

	Vector2 getCenteredTextPos(const Rectangle& box, float fontSize, const char* text)
	{
		Vector2 textSize = {MeasureTextEx(mainFont, text, fontSize, fontSpacing)};
		return {
			box.x + (box.width - textSize.x) / 2.0f,
			box.y + (box.height - textSize.y) / 2.0f
		};
	}

	void FPS()
	{
		int fps{GetFPS()};

		float roundness{0.0f};
		int segments{2};

		Rectangle box = {0.0f, 0.0f, 100.0f, 30.0f};

		DrawRectangleRounded(box, roundness, segments, colors::uiBGLight);
		DrawRectangleRoundedLines(box, roundness, segments, colors::textGray);

		float fontSize{static_cast<float>(mainFont.baseSize)};
		std::string text{"fps: " + std::to_string(fps)};

		Vector2 textPos{getCenteredTextPos(box, fontSize, text.c_str())};
		DrawTextEx(mainFont, text.c_str(), textPos, fontSize, fontSpacing, colors::textDark);
	}

	void lockDelay(const piece::Piece& activePiece, const board::Board& curBoard)
	{
		using ms = std::chrono::milliseconds;
		auto now{std::chrono::steady_clock::now()};

		float lockDuration{static_cast<float>(std::chrono::duration_cast<ms>(now - activePiece.lockStart()).count())};
		float tProgress{lockDuration / static_cast<float>(game::lockDelayMS.count())};

		std::string text{};

		if (!activePiece.isPositionValid(curBoard, {0, 1}))
		{
			text = std::format("[{}ms]", lockDuration);
		}
		else
		{
			text = std::format("[{}ms]", 0.0f);
			tProgress = 0.0f;
		}

		float barWidth{400.0f};
		float barHeight{20.0f};

		Rectangle box = {
			(static_cast<float>(screenWidth) / 2.0f) - (barWidth / 2.0f),
			(static_cast<float>(screenHeight) / 1.125f) - (barHeight / 2.0f),
			barWidth,
			barHeight
		};

		GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(colors::textBlueGray));
		GuiProgressBar(box, "lock delay", text.c_str(), &tProgress, 0.0f, 1.0f);
		GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(colors::textLight));
	}

	void bag()
	{
		float roundness{0.05f};
		int segments{1};

		float headerWidth{200.0f};
		float headerHeight{30.0f};

		float boxWidth{200.0f};
		float boxHeight{500.0f};

		Rectangle header = {
			(static_cast<float>(screenWidth) / 1.505f) - (headerWidth / 2.0f),
			(static_cast<float>(screenHeight) / 5.0f) - (headerHeight / 2.0f),
			headerWidth,
			headerHeight
		};

		Rectangle box = {
			(static_cast<float>(screenWidth) / 1.505f) - (boxWidth / 2.0f),
			(static_cast<float>(screenHeight) / 4.5f),
			boxWidth,
			boxHeight
		};

		DrawRectangleRounded(header, roundness, segments, colors::uiBGLight);
		DrawRectangleRoundedLines(header, roundness, segments, colors::textGray);

		DrawRectangleRounded(box, roundness, segments, colors::uiBGDark);
		DrawRectangleRoundedLinesEx(box, roundness, segments, borderThickness, colors::uiBGLight);

		float fontSize{static_cast<float>(mainFont.baseSize)};

		Vector2 textPos{getCenteredTextPos(header, fontSize, "Next")};
		DrawTextEx(mainFont, "Next", textPos, fontSize, fontSpacing, colors::textDark);
	}

	void bag3D(const board::Board& curBoard, int bagDisplayCount)
	{
		Vector3 defaultPosition{15.0f, 0.0f, -11.0f};

		float scale{0.9f};
		Vector3 cubeSize{Vector3Scale(curBoard.cubeSize(), scale)};

		float pieceSpacing{cubeSize.z * 3.0f};

		for (std::size_t i{}; i < static_cast<std::size_t>(bagDisplayCount); ++i)
		{
			Vector3 offset{0, 0, static_cast<float>(i) * pieceSpacing};
			Vector3 position{Vector3Add(defaultPosition, offset)};

			if ((curBoard.currentBag().currentIndex() + i) >= curBoard.currentBag().data().size())
			{
				pieceType::draw(curBoard.nextBag().peek(curBoard.currentBag().currentIndex() + i - 7), cubeSize, position);
			}
			else
			{
				pieceType::draw(curBoard.currentBag().peek(curBoard.currentBag().currentIndex() + i), cubeSize, position);
			}
		}
	}

	void hold()
	{
		float roundness{0.05f};
		int segments{1};

		float headerWidth{200.0f};
		float headerHeight{30.0f};

		float boxWidth{200.0f};
		float boxHeight{125.0f};

		Rectangle header = {
			(static_cast<float>(screenWidth) / 3.0f) - (headerWidth / 2.0f),
			(static_cast<float>(screenHeight) / 5.0f) - (headerHeight / 2.0f),
			headerWidth,
			headerHeight
		};

		Rectangle box = {
			(static_cast<float>(screenWidth) / 3.0f) - (boxWidth / 2.0f),
			(static_cast<float>(screenHeight) / 4.5f),
			boxWidth,
			boxHeight
		};

		DrawRectangleRounded(header, roundness, segments, colors::uiBGLight);
		DrawRectangleRoundedLines(header, roundness, segments, colors::textGray);

		DrawRectangleRounded(box, roundness, segments, colors::uiBGDark);
		DrawRectangleRoundedLinesEx(box, roundness, segments, borderThickness, colors::uiBGLight);

		float fontSize{static_cast<float>(mainFont.baseSize)};

		Vector2 textPos{getCenteredTextPos(header, fontSize, "Hold")};
		DrawTextEx(mainFont, "Hold", textPos, fontSize, fontSpacing, colors::textDark);
	}

	void hold3D(const board::Board& curBoard)
	{
		Vector3 defaultPosition{-16.0f, 0.0f, -11.0f};
		float scale{0.9f};
		Vector3 cubeSize{Vector3Scale(curBoard.cubeSize(), scale)};

		pieceType::draw(curBoard.heldPiece(), cubeSize, defaultPosition);
	}

	void pieceStats(const board::Board& curBoard)
	{
		float fontSize32{static_cast<float>(mainFontBig.baseSize)};
		float fontSize64{static_cast<float>(mainFontVeryBig.baseSize)};
		float spacing{2.0f};

		auto elapsedTime{Clock::now() - curBoard.startTime()};
		double totalSeconds{std::chrono::duration<double>(elapsedTime).count()};
		std::string textStats{std::format("{:.2f}/S", curBoard.pieceCount() / totalSeconds)};
		std::string textCount{std::format("{},", curBoard.pieceCount())};

		Vector2 size32{MeasureTextEx(mainFontBig, textStats.c_str(), fontSize32, spacing)};
		Vector2 size64{MeasureTextEx(mainFontVeryBig, textCount.c_str(), fontSize64, spacing)};

		Vector2 pos{
			(static_cast<float>(screenWidth) / 2.75f),
			(static_cast<float>(screenHeight) / 1.4f)
		};

		Vector2 pos32header{
			pos.x,
			pos.y - size32.y / 2.0f - size32.y
		};

		Vector2 pos32{
			pos.x,
			pos.y - size32.y / 2.0f
		};

		Vector2 pos64
		{
			pos.x - size64.x - 10.0f,
			pos.y - size64.y / 2.0f - 3.0f
		};

		DrawTextEx(mainFontBig, "Pieces", pos32header, fontSize32, fontSpacing, colors::uiBGLight);
		DrawTextEx(mainFontBig, textStats.c_str(), pos32, fontSize32, fontSpacing, colors::uiBGLight);
		DrawTextEx(mainFontVeryBig, textCount.c_str(), pos64, fontSize64, fontSpacing, colors::textWhite);
	}

	void time(const board::Board& curBoard)
	{
		float fontSize32{static_cast<float>(mainFontBig.baseSize)};
		float fontSize64{static_cast<float>(mainFontVeryBig.baseSize)};
		float spacing{2.0f};

		auto elapsedTime{Clock::now() - curBoard.startTime()};
		auto hours{
			std::chrono::duration_cast<std::chrono::hours>(elapsedTime)
		};
		elapsedTime -= hours;

		auto minutes
		{
			std::chrono::duration_cast<std::chrono::minutes>(elapsedTime)
		};
		elapsedTime -= minutes;

		auto seconds
		{
			std::chrono::duration_cast<std::chrono::seconds>(elapsedTime)
		};
		elapsedTime -= seconds;

		auto milliseconds
		{
			std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime)
		};

		std::string millis{std::format("{:03}", milliseconds.count())};
		std::string minutes_seconds{std::format("{:01}:{:02}.", minutes.count(), seconds.count())};

		Vector2 size32{MeasureTextEx(mainFontBig, millis.c_str(), fontSize32, spacing)};
		Vector2 size64{MeasureTextEx(mainFontVeryBig, minutes_seconds.c_str(), fontSize64, spacing)};

		Vector2 pos{
			(static_cast<float>(screenWidth) / 2.75f),
			(static_cast<float>(screenHeight) / 1.25f)
		};

		Vector2 pos32header{
			pos.x,
			pos.y - size32.y / 2.0f - size32.y
		};

		Vector2 pos32{
			pos.x,
			pos.y - size32.y / 2.0f
		};

		Vector2 pos64
		{
			pos.x - size64.x,
			pos.y - size64.y / 2.0f - 3.0f
		};

		DrawTextEx(mainFontBig, "Time", pos32header, fontSize32, fontSpacing, colors::uiBGLight);
		DrawTextEx(mainFontBig, millis.c_str(), pos32, fontSize32, fontSpacing, colors::uiBGLight);
		DrawTextEx(mainFontVeryBig, minutes_seconds.c_str(), pos64, fontSize64, fontSpacing, colors::textWhite);
	}

}
