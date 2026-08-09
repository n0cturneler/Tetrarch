#include "Input.hpp"
#include "Controls.hpp"

#include <raylib.h>

input::PieceActions input::getPieceAction(const PieceActions& curActions)
{	
	PieceActions actions{};
	actions.moveDirection = curActions.moveDirection;

	if (IsKeyPressed(controls::moveLeft))
	{
		actions.moveLeft = true;
		actions.moveDirection = MoveDirection::left;
	}

	if (IsKeyPressed(controls::moveRight))
	{
		actions.moveRight = true;
		actions.moveDirection = MoveDirection::right;
	}

	if (IsKeyDown(controls::moveLeft)) actions.holdLeft = true;
	if (IsKeyDown(controls::moveRight)) actions.holdRight = true;

	if (IsKeyReleased(controls::moveLeft) && IsKeyDown(controls::moveRight))
	{
		actions.moveDirection = MoveDirection::right;
	}
	if (IsKeyReleased(controls::moveRight) && IsKeyDown(controls::moveLeft))
	{
		actions.moveDirection = MoveDirection::left;
	}

	if (IsKeyPressed(controls::hardDrop)) actions.hardDrop = true;
	if (IsKeyDown(controls::softDrop)) actions.softDrop = true;

	if (IsKeyPressed(controls::rotLeft)) actions.rotLeft = true;
	if (IsKeyPressed(controls::rotRight)) actions.rotRight = true;

	if (IsKeyPressed(controls::rot180)) actions.rot180 = true;

	if (IsKeyPressed(controls::hold)) actions.holdPiece = true;

	return actions;
}
