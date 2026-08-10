#pragma once

#include <raylib.h>

#include <string_view>

namespace pieceType
{
	enum class PieceType
	{
		I,
		J,
		L,
		O,
		S,
		T,
		Z,
		none
	};

	void draw(PieceType type, Vector3 cubeSize, Vector3 position);

	std::string_view pieceTypeToString(PieceType type);
}

