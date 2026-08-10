#include "PieceType.hpp"

#include "PieceOffsets.hpp"
#include "Options.hpp"
#include "Board.hpp"
using namespace options;

#include <raylib.h>
#include <raymath.h>

#include <string_view>

namespace pieceType
{
	void draw(PieceType type, Vector3 cubeSize, Vector3 posOffset)
	{
		std::size_t pieceIndex{static_cast<std::size_t>(type)};
		std::size_t rotationState{0};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		Color mainColor{colors::piece[pieceIndex]};
		Color borderColor{colors::pieceBorder[pieceIndex]};

		for (const grid::Grid2D& offset : data)
		{
			Vector3 position = {
				posOffset.x + (static_cast<float>(offset.x) * cubeSize.x),
				posOffset.y,
				posOffset.z + (static_cast<float>(offset.y) * cubeSize.z)
			};

			DrawCube(position, cubeSize.x, cubeSize.y, cubeSize.z, mainColor);
			DrawCubeWires(position, cubeSize.x, cubeSize.y, cubeSize.z, borderColor);
		}
	}

	std::string_view pieceTypeToString(PieceType type)
	{
		switch (type)
		{
		case pieceType::PieceType::I: return "I";
		case pieceType::PieceType::J: return "J";
		case pieceType::PieceType::L: return "L";
		case pieceType::PieceType::O: return "O";
		case pieceType::PieceType::S: return "S";
		case pieceType::PieceType::T: return "T";
		case pieceType::PieceType::Z: return "Z";
		case pieceType::PieceType::none: return "X";

		default:
			break;
		}

	}
}


