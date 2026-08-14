#include "Camera.hpp"

#include "Options.hpp"
#include "Input.hpp"
using namespace options;

#include <raylib.h>

#include <iostream>
#include <algorithm>

namespace cam
{
	Camera3D initialize()
	{
		Camera3D camera{};
		camera.position = game::defaultCamPos;
		camera.target = game::worldOrigin;
		camera.up = game::camUpVector;

		camera.fovy = game::camFOVY;
		camera.projection = game::camProjection;

		return camera;
	}

	Vector3 update(const Vector3& position, const input::PieceActions& actions)
	{
		Vector3 newPosition{position};

		return newPosition;
	}
}
