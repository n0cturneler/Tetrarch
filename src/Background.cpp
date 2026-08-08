#include "Background.hpp"

#include <raylib.h>
#include <raymath.h>

#include "Options.hpp"
#include "Board.hpp"

#include <print>

using namespace options;

namespace background
{
	void draw()
	{
		// Buffer Grid
		for (int i{0}; i <= game::bufferRows; i++)
		{
			float zPos{game::cubeSize.z * static_cast<float>(i)};

			Vector3 startPos{game::halfWidth, 0.0f, zPos - game::fullHeight - game::halfHeight};
			Vector3 endPos{-game::halfWidth, 0.0f, zPos - game::fullHeight - game::halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
		}

		for (int i{0}; i <= game::columns; i++)
		{
			float xPos{game::cubeSize.x * static_cast<float>(i)};

			Vector3 startPos{xPos - game::halfWidth, 0.0f, -game::fullHeight + game::halfHeight};
			Vector3 endPos{xPos - game::halfWidth, 0.0f, -game::fullHeight - game::halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
		}

		// Play Grid
		for (int i{0}; i <= game::columns; i++)
		{
			float xPos{game::cubeSize.x * static_cast<float>(i)};

			Vector3 startPos{xPos - game::halfWidth, 0.0f, game::halfHeight};
			Vector3 endPos{xPos - game::halfWidth, 0.0f, -game::halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}

		for (int i{0}; i <= game::rows; i++)
		{
			float zPos{game::cubeSize.z * static_cast<float>(i)};

			Vector3 startPos{game::halfWidth, 0.0f, zPos - game::halfHeight};
			Vector3 endPos{-game::halfWidth, 0.0f, zPos - game::halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}
	}
}
