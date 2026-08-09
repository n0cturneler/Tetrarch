#pragma once

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
	};

	PieceActions getPieceAction(const PieceActions& curActions);
}
