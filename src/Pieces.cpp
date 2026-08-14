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

	void Piece::update(const input::PieceActions& actions, board::Board& curBoard)
	{
		TimePoint now{Clock::now()};
		m_DASState.lastPress = actions.lastPress;

		if (actions.holdPiece)
		{
			m_type = curBoard.holdPiece(*this);
			m_rotationState = 0;
			m_gridPos = curBoard.spawnPos();
		}

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

		if (actions.rotLeft)
		{
			int rotationOffset{-1};
			wallKick::Notation notation{getWallkickNotation(rotationOffset)};
			std::optional<grid::Grid2D> result{testWallkick(curBoard, rotationOffset, notation)};
			if (result)
			{
				m_rotationState += rotationOffset;
				m_gridPos = *result;
			}
		}
		if (actions.rotRight)
		{
			int rotationOffset{1};
			wallKick::Notation notation{getWallkickNotation(rotationOffset)};
			std::optional<grid::Grid2D> result{testWallkick(curBoard, rotationOffset, notation)};
			if (result)
			{
				m_rotationState += rotationOffset;
				m_gridPos = *result;
			}
		}
		if (actions.rot180)
		{
			int rotationOffset{2};
			wallKick::Notation notation{getWallkickNotation(rotationOffset)};
			std::optional<grid::Grid2D> result{testWallkick(curBoard, rotationOffset, notation)};
			if (result)
			{	
				m_rotationState += rotationOffset;
				m_gridPos = *result;
			}
		}
		m_rotationState = (m_rotationState + 4) % 4;

		MS currentDropRate{game::gravityMS};
		if (actions.softDrop) { currentDropRate = game::softdropMS; }

		MS elapsedSinceGravity = std::chrono::duration_cast<MS>(now - m_lastGravityTick);
		if (elapsedSinceGravity >= currentDropRate)
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
			m_gridPos = getHardDropPos(curBoard);
			curBoard.placePiece(*this);
			reset(curBoard.spawnPos(), curBoard.getNextPieceType(), now);
		}

		MS elapsedDurationLock = std::chrono::duration_cast<MS>(now - m_lockStart);
		if (!isPositionValid(curBoard, {0, 1}))
		{
			if (elapsedDurationLock >= game::lockDelayMS)
			{
				curBoard.placePiece(*this);
				reset(curBoard.spawnPos(), curBoard.getNextPieceType(), now);
			}
		}
	}

	void Piece::reset(grid::Grid2D spawnPos, pieceType::PieceType nextPieceType, TimePoint now)
	{
		m_lastGravityTick = now;
		m_lockStart = now;
		m_gridPos = spawnPos;
		m_rotationState = 0;
		m_type = {nextPieceType};
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

		if (m_type == pieceType::PieceType::none) { return; }

		std::size_t pieceIndex{static_cast<std::size_t>(m_type)};
		std::size_t rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		Color mainColor{colors::piece[pieceIndex]};
		Color borderColor{colors::pieceBorder[pieceIndex]};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPosition = {m_gridPos + offset};
			Vector3 position = {curBoard.boardToWorld(gridPosition)};

			/*if (curBoard.isOutOfBounds({gridPosition.x, gridPosition.y - static_cast<int>(curBoard.bufferRows())}))
			{
				mainColor = Fade(mainColor, 0.15f);
				borderColor = Fade(borderColor, 1.0f);
			}*/

			DrawCube(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, mainColor);
			DrawCubeWires(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, borderColor);
		}
	}

	void Piece::drawGhostPiece(const board::Board& curBoard) const
	{	
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		if (m_type == pieceType::PieceType::none) { return; }

		auto pieceIndex{static_cast<std::size_t>(m_type)};
		auto rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		Color mainColor{colors::piece[pieceIndex]};
		Color darkColor = ColorLerp(mainColor, BLACK, 0.8f);

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPosition = {getHardDropPos(curBoard) + offset};
			Vector3 position = {curBoard.boardToWorld(gridPosition)};

			if (!curBoard.isOutOfBounds(gridPosition))
			{
				DrawCube(position, curBoard.cubeSize().x / 2, curBoard.cubeSize().y / 2, curBoard.cubeSize().z / 2, mainColor);
				DrawCube(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, Fade(darkColor, 0.75f));
				DrawCubeWires(position, curBoard.cubeSize().x, curBoard.cubeSize().y, curBoard.cubeSize().z, mainColor);
			}
		}
	}

	grid::Grid2D Piece::getHardDropPos(const board::Board& curBoard) const
	{
		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(m_rotationState >= 0 && m_rotationState <= 3);

		auto pieceIndex{static_cast<std::size_t>(m_type)};
		auto rotationState{static_cast<std::size_t>(m_rotationState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		int maxY{static_cast<int>(curBoard.fullRows())};

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

	wallKick::Notation Piece::getWallkickNotation(int rotationOffset) const
	{
		int testState{m_rotationState + rotationOffset};
		const int currentState{m_rotationState};

		testState = (testState + 4) % 4;

		assert(testState >= 0 && testState <= 3);

		// L is 3, R is 1, 2 is ofc 2

		switch (currentState)
		{
		case 0:
		{
			if (testState == 3) return wallKick::Notation::Zero_to_L;
			if (testState == 1) return wallKick::Notation::Zero_to_R;
			break;
		}
		case 1:
		{
			if (testState == 2)	return wallKick::Notation::R_to_Two;
			if (testState == 0) return wallKick::Notation::R_to_Zero;
			break;
		}
		case 2:
		{
			if (testState == 3) return wallKick::Notation::Two_to_L;
			if (testState == 1) return wallKick::Notation::Two_to_R;
			break;
		}
		case 3:
		{
			if (testState == 2) return wallKick::Notation::L_to_Two;
			if (testState == 0) return wallKick::Notation::L_to_Zero;
			break;
		}
		default:
			break;
		}
		return wallKick::Notation::Invalid;
	}

	std::optional<grid::Grid2D> Piece::testWallkick(const board::Board& curBoard, int rotationOffset, wallKick::Notation notation) const
	{	
		if (notation == wallKick::Notation::Invalid) return std::nullopt;

		int testState{m_rotationState + rotationOffset};
		testState = (testState + 4) % 4;

		assert(m_type != pieceType::PieceType::none);
		assert(static_cast<int>(m_type) <= 6);
		assert(testState >= 0 && testState <= 3);

		const wallKick::Data& wallkickData{
			m_type == pieceType::PieceType::I
				? wallKick::I
				: wallKick::JLSTZ
		};

		if (wallkickData.empty()) return std::nullopt;

		const std::size_t pieceIndex{static_cast<std::size_t>(m_type)};
		const std::size_t rotationState{static_cast<std::size_t>(testState)};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		for (std::size_t i{0}; i < wallkickData[0].size(); ++i)
		{
			grid::Grid2D wallkickOffset = wallkickData[static_cast<std::size_t>(notation)][i];

			bool valid{true};

			for (const grid::Grid2D& offset : data)
			{
				grid::Grid2D testPos = {m_gridPos + offset + wallkickOffset};

				if (curBoard.isColliding(testPos) || curBoard.isOutOfBounds(testPos))
				{	
					valid = false;
					break;
				}
			}

			if (valid)
			{
				return m_gridPos + wallkickOffset;
			}

		}
		return std::nullopt;
	}
}
