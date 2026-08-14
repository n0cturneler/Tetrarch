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
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	// SetConfigFlags(FLAG_FULLSCREEN_MODE);

	InitWindow(0, 0, "3DTRTRIS");
	MaximizeWindow();

	UI::initialize();

	SetTargetFPS(options::video::frameRate);

	Camera3D camera{cam::initialize()};

	board::Board curBoard{
		options::game::rows, 
		options::game::bufferRows, 
		options::game::columns, 
		options::game::scale,
		options::game::cubeSize, 
		options::game::gridSpawn,
		options::game::worldOrigin
	};

	piece::Piece activePiece{curBoard.spawnPos(), curBoard.getNextPieceType()};
	input::PieceActions currentAction{};

	while (!WindowShouldClose())
	{
		currentAction = {input::getPieceAction(currentAction)};
		activePiece.update(currentAction, curBoard);

		curBoard.updatePosition(currentAction);

		BeginDrawing();
		ClearBackground(options::colors::background);

			UI::bag();
			UI::hold();

		BeginMode3D(camera);
			
			curBoard.drawBackground(true);
			curBoard.drawSpawnLocation();
				
			activePiece.drawGhostPiece(curBoard);
			curBoard.draw();
			activePiece.draw(curBoard);

			UI::bag3D(curBoard, options::game::bagDisplayCount);
			UI::hold3D(curBoard);

		EndMode3D();
			
			UI::FPS();
			UI::pieceStats(curBoard);
			UI::time (curBoard);
			UI::lockDelay(activePiece, curBoard);
			
		EndDrawing();
	}

	CloseWindow();
}
