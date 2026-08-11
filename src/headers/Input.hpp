#pragma once

#include <chrono>

namespace piece
{
	class Piece;
}

namespace input
{	
	enum class MoveDirection
	{
		none,
		left,
		right
	};

	struct PieceActions
	{	
		MoveDirection moveDirection{};

		bool moveLeft{};
		bool moveRight{};

		bool holdLeft{};
		bool holdRight{};

		bool rotLeft{};
		bool rotRight{};
		bool rot180{};

		bool softDrop{};
		bool hardDrop{};

		bool holdPiece{};

		std::chrono::steady_clock::time_point lastPress{};
	};

	PieceActions getPieceAction(const PieceActions& curActions);
}
