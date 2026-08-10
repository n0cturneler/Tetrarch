#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif 

#include "Options.hpp"
#include "PieceType.hpp"
#include "Pieces.hpp"
#include "Bag.hpp"
#include "Camera.hpp"
#include "UI.hpp"
#include "Cell.hpp"
#include "Input.hpp"
#include "Board.hpp"

#include "Random.hpp"

#include <raylib.h>

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

	UI::initialize();

	Camera3D camera{cam::initialize()};

	bag::Bag currentBag{Random::mt};
	bag::Bag nextBag{Random::mt};

	board::Board curBoard{options::game::rows, 
		options::game::bufferRows, 
		options::game::columns, 
		options::game::scale,
		options::game::cubeSize, 
		options::game::gridSpawn,
		options::game::worldOrigin
	};

	piece::Piece activePiece{curBoard.spawnPos(), currentBag.getNextpieceType(nextBag)};

	input::PieceActions currentAction{};

	while (!WindowShouldClose())
	{
		float mouseWheelMovement = GetMouseWheelMove();
		camera.position = cam::update(camera.position, mouseWheelMovement);

		currentAction = {input::getPieceAction(currentAction)};
		activePiece.update(currentAction, curBoard, currentBag, nextBag);

		BeginDrawing();
		ClearBackground(options::colors::background);

		BeginMode3D(camera);
			
		curBoard.drawBackground(true);

		activePiece.drawGhostPiece(curBoard);
		curBoard.draw();
		activePiece.draw(curBoard);
			
		EndMode3D();

		UI::FPS();
		UI::drawBag(currentBag, nextBag);
		UI::lockDelay(activePiece, curBoard);
			
		EndDrawing();
	}

	CloseWindow();
}
