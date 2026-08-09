#include "Pieces.hpp"

#include "Grid.hpp"
#include "PieceType.hpp"
#include "PieceOffsets.hpp"
#include "Bag.hpp"
#include "Wallkicks.hpp"
#include "Board.hpp"
#include "Options.hpp"
using namespace options;

#include "Cell.hpp"

#include "Random.hpp"

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <vector>
#include <array>
#include <optional>

namespace piece
{
	Piece::Piece(grid::Grid2D spawnPos, pieceType::PieceType type, int rotationState)
		: m_gridPos{spawnPos}, m_type{type}, m_rotationState{rotationState}
	{
	}

	void Piece::update(const input::PieceActions& actions, board::Board& curBoard, bag::Bag& currentBag, bag::Bag& nextBag)
	{
		TimePoint now{Clock::now()};

		if (actions.moveLeft || actions.moveRight)
		{
			m_DASState.lastPress = now;

			grid::Grid2D movePos{};
			if (actions.moveDirection == input::MoveDirection::left) { movePos = {-1, 0}; }
			if (actions.moveDirection == input::MoveDirection::right) { movePos = {1, 0}; }

			if (isPositionValid(curBoard, movePos))
			{
				m_DASState.lastMove = now;
				m_lockStart = now;

				m_gridPos.x += movePos.x;
			}
		}

		if (actions.holdLeft || actions.holdRight)
		{
			grid::Grid2D movePos{};
			if (actions.moveDirection == input::MoveDirection::left) { movePos = {-1, 0}; }
			if (actions.moveDirection == input::MoveDirection::right) { movePos = {1, 0}; }

			MS elapsedSincePress = std::chrono::duration_cast<MS>(now - m_DASState.lastPress);
			MS elapsedSinceMove = std::chrono::duration_cast<MS>(now - m_DASState.lastMove);

			if (elapsedSincePress >= game::DAS && elapsedSinceMove >= game::ARR
				&& isPositionValid(curBoard, movePos)
				)
			{
				m_DASState.lastMove = now;
				m_lockStart = now;
				m_gridPos.x += movePos.x;
			}
		}

		int oldRotationState{m_rotationState};
		grid::Grid2D oldPosition{m_gridPos};

		if (actions.rotLeft)
		{
			m_rotationState -= 1;
			//wallKick::Notation currentNotation{getWallkickNotation(-1)};
			//m_gridPos = testWallkick(currentNotation, staticPieces);
		}
		if (actions.rotRight)
		{
			m_rotationState += 1;
			//wallKick::Notation currentNotation{getWallkickNotation(1)};
			//m_gridPos = testWallkick(currentNotation, staticPieces);
		}
		if (actions.rot180)
		{
			m_rotationState += 2;
			//wallKick::Notation currentNotation{getWallkickNotation(2)};
			//m_gridPos = testWallkick(currentNotation, staticPieces);
		}
		m_rotationState = (m_rotationState + 4) % 4;
		//if (oldPosition == m_gridPos) m_rotationState = oldRotationState;

		MS currentDropRate{game::gravityMS};
		if (actions.softDrop) { currentDropRate = game::softdropMS; }

		MS duration_grav = std::chrono::duration_cast<MS>(now - m_lastGravityTick);
		if (duration_grav >= currentDropRate)
		{
			if (isPositionValid(curBoard, {0, 1}))
			{
				m_lastGravityTick = now;
				m_lockStart = now;
				m_gridPos.y += 1;
			}
		}

		if (actions.hardDrop)
		{
			m_lastGravityTick = now;
			m_lockStart = now;
			m_gridPos = getHardDropPos(curBoard);
			curBoard.placePiece(*this);
			reset(currentBag, nextBag, curBoard);
		}

		MS duration_lock = std::chrono::duration_cast<MS>(now - m_lockStart);
		if (!isPositionValid(curBoard, {0, 1}))
		{
			if (duration_lock >= game::lockDelayMS)
			{
				m_lastGravityTick = now;
				m_lockStart = now;
				curBoard.placePiece(*this);
				reset(currentBag, nextBag, curBoard);
			}
		}
	}

	bool Piece::isPositionValid(const board::Board& curBoard, grid::Grid2D testOffset) const
	{
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		auto pieceIndex{static_cast<std::size_t>(m_type)};
		auto rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPos{offset + m_gridPos};
			grid::Grid2D testPos{gridPos + testOffset};

			if (curBoard.isColliding(testPos) || curBoard.isOutOfBounds(testPos))
			{
				return false;
			}
		}
		return true;
	}

	void Piece::draw(const board::Board& curBoard) const
	{
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		std::size_t pieceIndex{static_cast<std::size_t>(m_type)};
		std::size_t rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPosition = {m_gridPos + offset};
			Vector3 position = {curBoard.boardToWorld(gridPosition)};

			Color mainColor{colors::piece[pieceIndex]};
			Color borderColor{colors::pieceBorder[pieceIndex]};

			if (curBoard.isOutOfBounds({gridPosition.x, gridPosition.y - static_cast<int>(curBoard.bufferRows())}))
			{
				mainColor = Fade(mainColor, 0.15f);
				borderColor = Fade(borderColor, 1.0f);
			}

			DrawCube(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, mainColor);
			DrawCubeWires(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, borderColor);
		}
	}

	void Piece::drawGhostPiece(const board::Board& curBoard) const
	{
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		auto pieceIndex{static_cast<std::size_t>(m_type)};
		auto rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPosition = {getHardDropPos(curBoard) + offset};
			Vector3 position = {curBoard.boardToWorld(gridPosition)};

			if (!curBoard.isOutOfBounds(gridPosition))
			{
				Color mainColor{colors::piece[pieceIndex]};
				Color darkColor = ColorLerp(mainColor, BLACK, 0.8f);

				DrawCube(position, curBoard.cubeSize().x / 2, curBoard.cubeSize().y / 2, curBoard.cubeSize().z / 2, mainColor);
				DrawCube(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, Fade(darkColor, 0.75f));
				DrawCubeWires(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, mainColor);
			}
		}
	}

	void Piece::reset(bag::Bag& currentBag, bag::Bag& nextBag, const board::Board& curBoard)
	{
		m_gridPos = curBoard.spawnPos();
		m_rotationState = 0;
		m_type = {currentBag.getNextpieceType(nextBag)};
	}

	grid::Grid2D Piece::getHardDropPos(const board::Board& curBoard) const
	{
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		auto pieceIndex{static_cast<std::size_t>(m_type)};
		auto rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		int maxY = {static_cast<int>(curBoard.fullRows())};

		for (int y{0}; y <= maxY; ++y)
		{
			bool collided{false};

			for (const grid::Grid2D& offset : data)
			{
				grid::Grid2D testPos{offset + m_gridPos};
				testPos.y += y;

				if (curBoard.isColliding(testPos) || testPos.y >= maxY)
				{
					collided = true;
					break;
				}
			}

			if (collided)
			{
				return {m_gridPos.x, m_gridPos.y + y - 1};
			}
		}
		return m_gridPos;
	}

	wallKick::Notation Piece::getWallkickNotation(int offset) const
	{
		int testState{m_rotationState + offset};
		const int currentState{m_rotationState};

		testState = (testState + 4) % 4;

		assert(testState >= 0 && testState <= 3);
		assert(testState >= 0 && testState <= 3);

		// L is 3, R is 1, 2 is ofc 2

		switch (currentState)
		{
		case 0:
		{
			if (testState == 3)
			{
				return wallKick::Notation::Zero_to_L;
			}
			if (testState == 1)
			{
				return wallKick::Notation::Zero_to_R;
			}
		}
		case 1:
		{
			if (testState == 2)
			{
				return wallKick::Notation::R_to_Two;
			}
			if (testState == 0)
			{
				return wallKick::Notation::R_to_Zero;
			}
		}
		case 2:
		{
			if (testState == 3)
			{
				return wallKick::Notation::Two_to_L;
			}
			if (testState == 1)
			{
				return wallKick::Notation::Two_to_R;
			}
		}
		case 3:
		{
			if (testState == 2)
			{
				return wallKick::Notation::L_to_Two;
			}
			if (testState == 0)
			{
				return wallKick::Notation::L_to_Zero;
			}
		}
		default:
			break;
		}
		return wallKick::Notation::Zero_to_R;
	}

	std::optional<grid::Grid2D> Piece::testWallkick(wallKick::Notation notation, const board::Board& curBoard) const
	{
		for (int i{0}; i <= (game::wallkickAmount - 1); ++i)
		{
			if (m_type == pieceType::PieceType::I)
			{
				grid::Grid2D offset = wallKick::I[static_cast<std::size_t>(notation)][static_cast<std::size_t>(i)];
				grid::Grid2D testPos{offset + m_gridPos};

				if (!curBoard.isColliding(testPos))
				{
					return testPos;
				}
			}
			else
			{
				grid::Grid2D offset = wallKick::JLSTZ[static_cast<std::size_t>(notation)][static_cast<std::size_t>(i)];
				grid::Grid2D testPos{offset + m_gridPos};

				if (!curBoard.isColliding(testPos))
				{
					return testPos;
				}
			}
		}
		return std::nullopt;
	}
}
