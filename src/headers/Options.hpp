#pragma once

#include "Grid.hpp"
#include <raylib.h>

#include <array>
#include <chrono>

namespace options
{
	namespace video
	{
		inline constexpr int frameRate{120};
	}

	namespace game
	{
		//Game
		inline constexpr int rows{20};
		inline constexpr int columns{10};

		inline constexpr int bufferRows{20};

		inline constexpr int bagDisplayCount{5};
		
		inline constexpr int wallkickAmount{5};

		inline constexpr float scale{1.0};

		// MS
		inline constexpr std::chrono::milliseconds gravityMS{1000};

		inline constexpr std::chrono::milliseconds DAS{133}; 
		inline constexpr std::chrono::milliseconds ARR{33};

		inline constexpr std::chrono::milliseconds lockDelayMS{1500};
		inline constexpr std::chrono::milliseconds lockDelayLimit{15};

		inline constexpr std::chrono::milliseconds softdropMS{33}; 

		//Camera
		inline constexpr float camScrollRate{5.0f};
		inline constexpr float camFOVY{45.0f};

		inline constexpr Vector3 defaultCamPos{0.0f, 65.0f, 0.01f};
		inline constexpr Vector3 camUpVector{0.0f, 1.0f, 0.0f};
		inline constexpr CameraProjection camProjection{CAMERA_PERSPECTIVE};

		//Cube
		inline constexpr Vector3 cubeSize{1.75f, 1.75f, 1.75f};

		//World
		inline constexpr Vector3 worldOrigin{0.0f, 0.0f, 0.0f};

		//Grid
		inline constexpr grid::Grid2D gridSpawn{4, -1};

	}

	namespace colors
	{
		inline constexpr Color background{13, 18, 18, 255};

		inline constexpr Color backgroundLines{100, 100, 100, 255};
		inline constexpr Color backgroundBufferLines{50, 50, 50, 255};

		// UI
		inline constexpr Color uiBGLight{110, 175, 255, 255};
		inline constexpr Color uiBGDark{0, 15, 20, 255};

		inline constexpr Color uiAltLight{118, 214, 157, 255};
		inline constexpr Color uiAltDark{55, 97, 72, 255};

		inline constexpr Color textLight{180, 215, 255, 255};
		inline constexpr Color textDark{20, 25, 30, 255};

		inline constexpr Color textGray{140, 140, 140, 255};
		inline constexpr Color textBlueGray{83, 109, 140, 255};

		//PIECES
		inline constexpr Color I{5, 255, 205, 255};
		inline constexpr Color I_Border{0, 80, 60, 255};

		inline constexpr Color J{30, 50, 255, 255};
		inline constexpr Color J_Border{15, 40, 80, 255};

		inline constexpr Color L{255, 170, 25, 255};
		inline constexpr Color L_Border{80, 30, 5, 255};

		inline constexpr Color O{255, 255, 45, 255};
		inline constexpr Color O_Border{80, 80, 20, 255};

		inline constexpr Color S{55, 255, 8, 255};
		inline constexpr Color S_Border{55, 50, 8, 255};

		inline constexpr Color T{200, 70, 255, 255};
		inline constexpr Color T_Border{120, 10, 175, 255};

		inline constexpr Color Z{255, 40, 50, 255};
		inline constexpr Color Z_Border{140, 20, 30, 255};

		inline constexpr std::array<Color, 7> piece = {
			I,
			J,
			L,
			O,
			S,
			T,
			Z
		};

		inline constexpr std::array<Color, 7> pieceBorder = {
			I_Border,
			J_Border,
			L_Border,
			O_Border,
			S_Border,
			T_Border,
			Z_Border
		};
	}
}
