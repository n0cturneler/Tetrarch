#include "Pieces.hpp"

#include "Grid.hpp"
#include "PieceType.hpp"
#include "PieceOffsets.hpp"
#include "Bag.hpp"
#include "Wallkicks.hpp"
#include "Options.hpp"
using namespace options;

#include "Cell.hpp"
using Board = std::array<
	std::array<cell::Cell, options::game::columns>,
	options::game::rows
>;

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

piece::Piece::Piece(grid::Grid2D spawnPos, pieceType::PieceType type, int rotationState)
	: m_gridPos{spawnPos}, m_type{type}, m_rotationState{rotationState}
{
}

void piece::Piece::update(const input::PieceActions& actions, Board& staticPieces, bag::Bag& currentBag, bag::Bag& nextBag)
{
	using ms = std::chrono::milliseconds;
	auto now{std::chrono::steady_clock::now()};
	 
	if (actions.movLeft)
	{
		m_leftState.lastPress = now;
		
		if (isPositionValid(staticPieces, {-1, 0}))
		{	
			m_lockStart = now;
			m_gridPos.x -= 1;
		} 
	}
	if (actions.movRight)
	{
		m_rightState.lastPress = now;

		if (isPositionValid(staticPieces, {1, 0}))
		{	
			m_lockStart = now;
			m_gridPos.x += 1;
		}
	}

	if (actions.holdLeft)
	{
		auto duration_l = std::chrono::duration_cast<ms>(now - m_leftState.lastPress);
		auto duration_arr = std::chrono::duration_cast<ms>(now - m_leftState.lastMove);

		if (duration_l.count() >= game::DAS && duration_arr.count() >= game::ARR)
		{
			m_leftState.lastMove = now;
			if (isPositionValid(staticPieces, {-1, 0}))
			{
				m_lockStart = now;
				m_gridPos.x -= 1;
			}
		}
	}
	if (actions.holdRight)
	{
		auto duration_r = std::chrono::duration_cast<ms>(now - m_rightState.lastPress);
		auto duration_arr = std::chrono::duration_cast<ms>(now - m_rightState.lastMove); 

		if (duration_r.count() >= game::DAS && duration_arr.count() >= game::ARR)
		{
			m_rightState.lastMove = now;
			if (isPositionValid(staticPieces, {1, 0}))
			{
				m_lockStart = now;
				m_gridPos.x += 1;
			}
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

	int currentDropRate{game::gravityMS};
	if (actions.softDrop) { currentDropRate = game::softdropMS; }

	auto duration_grav = std::chrono::duration_cast<ms>(now - m_lastGravityTick);
	if (duration_grav.count() >= currentDropRate)
	{
		if (isPositionValid(staticPieces, {0, 1}))
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
		m_gridPos = getHardDropPos(staticPieces);
		setStaticData(staticPieces);
		reset(currentBag, nextBag);
	}

	auto duration_lock = std::chrono::duration_cast<ms>(now - m_lockStart);
	if (!isPositionValid(staticPieces, {0, 1}))
	{	
		if (duration_lock.count() >= game::lockDelayMS)
		{
			m_lockStart = now;
			setStaticData(staticPieces);
			reset(currentBag, nextBag);
		}
	}
}

wallKick::Notation piece::Piece::getWallkickNotation(int offset) const
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

grid::Grid2D piece::Piece::testWallkick(wallKick::Notation notation, const Board& staticPieces) const
{	
	for (int i{0}; i <= (game::wallkickAmount - 1); ++i)
	{
		if (m_type == pieceType::PieceType::I)
		{
			grid::Grid2D offset = wallKick::I[static_cast<std::size_t>(notation)][static_cast<std::size_t>(i)];
			grid::Grid2D testPos{grid::add(offset, m_gridPos)};

			if (not isColliding(staticPieces, testPos))
			{
				return testPos;
			}
		}
		else
		{	
			grid::Grid2D offset = wallKick::JLSTZ[static_cast<std::size_t>(notation)][static_cast<std::size_t>(i)];
			grid::Grid2D testPos{grid::add(offset, m_gridPos)};

			if (not isColliding(staticPieces, testPos))
			{
				return testPos;
			}
		}
	}
	return {};
}

void piece::Piece::draw() const
{
	assert(m_type != pieceType::PieceType::none);
	assert(static_cast<int>(m_type) <= 6);
	assert(m_rotationState >= 0 && m_rotationState <= 3);

	auto pieceIndex{static_cast<std::size_t>(m_type)};
	auto rotationState{static_cast<std::size_t>(m_rotationState)};
	const auto& data{pieceData::Data[pieceIndex][rotationState]};

	for (const grid::Grid2D& offset : data)
	{
		grid::Grid2D gridPosition = grid::add(m_gridPos, offset);
		Vector3 position = grid::gridToWorld(gridPosition);

		Color mainColor{colors::piece[pieceIndex]};
		Color borderColor{colors::pieceBorder[pieceIndex]};

		if (isOutOfBounds(gridPosition))
		{
			mainColor = Fade(mainColor, 0.15f);
			borderColor = Fade(borderColor, 1.0f);
		}

		DrawCube(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, mainColor);
		DrawCubeWires(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, borderColor);
	}
}

void piece::Piece::drawGhostPiece(const Board& staticPieces) const
{
	assert(m_type != pieceType::PieceType::none);
	assert(static_cast<int>(m_type) <= 6);
	assert(m_rotationState >= 0 && m_rotationState <= 3);

	auto pieceIndex{static_cast<std::size_t>(m_type)};
	auto rotationState{static_cast<std::size_t>(m_rotationState)};
	const auto& data{pieceData::Data[pieceIndex][rotationState]};

	for (const grid::Grid2D& offset : data)
	{
		grid::Grid2D gridPosition = grid::add(getHardDropPos(staticPieces), offset);
		Vector3 position = grid::gridToWorld(gridPosition);

		if (!isOutOfBounds(gridPosition))
		{
			Color mainColor{colors::piece[pieceIndex]};
			Color darkColor = ColorLerp(mainColor, BLACK, 0.8f);

			DrawCube(position, game::cubeSize.x/2, game::cubeSize.y/2, game::cubeSize.z/2, mainColor);
			DrawCube(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, Fade(darkColor, 0.75f));
			DrawCubeWires(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, mainColor);
		}
	}
}

void piece::Piece::reset(bag::Bag& currentBag, bag::Bag& nextBag)
{
	m_gridPos = game::gridSpawn;
	m_rotationState = 0;
	m_type = {currentBag.getNextpieceType(nextBag)};
}

bool piece::Piece::isColliding(const Board& staticPieces, grid::Grid2D testPos) const
{	
	if (testPos.x >= 0 &&
		testPos.x < game::columns &&
		testPos.y >= 0 &&
		testPos.y < game::rows)
	{
		if (staticPieces[static_cast<std::size_t>(testPos.y)][static_cast<std::size_t>(testPos.x)].type != pieceType::PieceType::none)
		{
			return true;
		}
	}
	return false;
}

bool piece::Piece::isOutOfBounds(grid::Grid2D testPos) const
{
	if (testPos.x >= 0 &&
		testPos.x < game::columns &&
		testPos.y > -game::bufferRows &&
		testPos.y < game::rows)
	{
		return false;
	}
	return true;
}

bool piece::Piece::isPositionValid(const Board& staticPieces, grid::Grid2D testOffset) const
{
	assert(m_type != pieceType::PieceType::none);
	assert(static_cast<int>(m_type) <= 6);
	assert(m_rotationState >= 0 && m_rotationState <= 3);

	auto pieceIndex{static_cast<std::size_t>(m_type)};
	auto rotationState{static_cast<std::size_t>(m_rotationState)};
	const auto& data{pieceData::Data[pieceIndex][rotationState]};

	for (const grid::Grid2D& offset : data)
	{
		grid::Grid2D gridPos{grid::add(offset, m_gridPos)};
		grid::Grid2D testPos{grid::add(gridPos, testOffset)};

		if (isColliding(staticPieces, testPos) ||
			isOutOfBounds(testPos)
			)
		{
			return false;
		}
	}
	return true;
}

grid::Grid2D piece::Piece::getHardDropPos(const Board& staticPieces) const
{
	assert(m_type != pieceType::PieceType::none);
	assert(static_cast<int>(m_type) <= 6);
	assert(m_rotationState >= 0 && m_rotationState <= 3);

	auto pieceIndex{static_cast<std::size_t>(m_type)};
	auto rotationState{static_cast<std::size_t>(m_rotationState)};
	const auto& data{pieceData::Data[pieceIndex][rotationState]};

	int maxY{game::rows};

	for (int y{0}; y <= (maxY - m_gridPos.y); ++y)
	{
		bool collided{false};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D testPos{grid::add(offset, m_gridPos)};
			testPos.y += y;

			if (isColliding(staticPieces, testPos) || testPos.y >= maxY)
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

void piece::Piece::setStaticData(Board& staticPieces) const
{
	assert(m_type != pieceType::PieceType::none);
	assert(static_cast<int>(m_type) <= 6);
	assert(m_rotationState >= 0 && m_rotationState <= 3);

	auto pieceIndex{static_cast<std::size_t>(m_type)};
	auto rotationState{static_cast<std::size_t>(m_rotationState)};
	const auto& data{pieceData::Data[pieceIndex][rotationState]};

	for (const grid::Grid2D& offset : data)
	{	
		grid::Grid2D testPos{grid::add(offset, m_gridPos)};

		if (testPos.x >= 0 &&
			testPos.x < game::columns &&
			testPos.y >= 0 &&
			testPos.y < game::rows)
		{
			if (staticPieces[static_cast<std::size_t>(testPos.y)][static_cast<std::size_t>(testPos.x)].type == pieceType::PieceType::none)
			{
				staticPieces[static_cast<std::size_t>(testPos.y)][static_cast<std::size_t>(testPos.x)].type = m_type;
			}
		}
	}
}

void piece::drawStatic(const Board& staticPieces)
{
	for (int y{0}; y < game::rows; ++y)
	{	
		for (int x{0}; x < game::columns; ++x)
		{
			if (staticPieces[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].type != pieceType::PieceType::none)
			{	
				auto pieceIndex{static_cast<std::size_t>(staticPieces[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].type)};

				Color mainColor{colors::piece[pieceIndex]};
				Color borderColor{colors::pieceBorder[pieceIndex]};

				Vector3 position = grid::gridToWorld({x, y});

				DrawCube(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, mainColor);
				DrawCubeWires(position, game::cubeSize.x, game::cubeSize.y, game::cubeSize.z, borderColor);
			}
		}
	}
}
