#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif 

#include "Options.hpp"
#include "Background.hpp"
#include "PieceType.hpp"
#include "Pieces.hpp"
#include "Bag.hpp"
#include "Camera.hpp"
#include "UI.hpp"
#include "Cell.hpp"
#include "Input.hpp"

#include "Random.hpp"

#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

#include <iostream> 
#include <vector>
#include <cstdint>
#include <chrono>
#include <random>

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	// SetConfigFlags(FLAG_FULLSCREEN_MODE);

	InitWindow(0, 0, "3DTRTRIS");
	MaximizeWindow();

	SetTargetFPS(options::video::frameRate);

	rlImGuiSetup(true);
	UI::initialize();

	Camera3D camera{cam::initialize()};

	bag::Bag currentBag{Random::mt};
	bag::Bag nextBag{Random::mt};

	piece::Piece activePiece{options::game::gridSpawn, currentBag.getNextpieceType(nextBag)};

	piece::Board staticPieces{};

	while (!WindowShouldClose())
	{
		[[maybe_unused]] float dt{GetFrameTime()};

		//float mouseWheelMovement = GetMouseWheelMove();
		//camera.position = cam::update(camera.position, mouseWheelMovement);

		input::PieceActions currentAction{input::getPieceAction()};
		activePiece.update(currentAction, staticPieces, currentBag, nextBag);

		BeginDrawing();
		ClearBackground(options::colors::background);

		BeginMode3D(camera);

			Background::draw();

			piece::drawStatic(staticPieces);
			activePiece.drawGhostPiece(staticPieces);
			activePiece.draw();

		EndMode3D();

		rlImGuiBegin();
			
			UI::FPS();
			UI::drawBag(currentBag, nextBag);
			UI::lockDelay(activePiece, staticPieces);
			
		rlImGuiEnd();
		EndDrawing();
	}

	rlImGuiShutdown();
	CloseWindow();
}
