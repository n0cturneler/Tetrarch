#pragma once

#include "Grid.hpp"
#include "Options.hpp"
#include "PieceType.hpp"
#include "Bag.hpp"
#include "Cell.hpp"
#include "Input.hpp"
#include "Wallkicks.hpp"
#include "Board.hpp"

#include <raylib.h>

#include <iostream> 
#include <vector>
#include <array>
#include <chrono>
#include <optional>

namespace piece
{	
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using MS = std::chrono::milliseconds;

	class Piece
	{
	public:
		struct DASState
		{	
			TimePoint lastPress{Clock::now()};
			TimePoint lastMove{Clock::now()};
		};

		Piece(grid::Grid2D spawnPos, pieceType::PieceType type, int rotationState = 0);

		pieceType::PieceType type() const { return m_type; }
		int rotationState() const { return m_rotationState; }
		grid::Grid2D gridPos() const { return m_gridPos; }

		TimePoint lockStart() const { return m_lockStart; }

		void update(const input::PieceActions& actions, board::Board& curBoard);

		void draw(const board::Board& curBoard) const;
		void drawGhostPiece(const board::Board& curBoard) const;

		bool isPositionValid(const board::Board& curBoard, grid::Grid2D testOffset = {}) const;

	private:
		void reset(grid::Grid2D spawnPos, pieceType::PieceType nextPieceType, TimePoint now);

		std::optional<grid::Grid2D> testWallkick(const board::Board& curBoard, int rotationOffset, wallKick::Notation notation) const;
		wallKick::Notation getWallkickNotation(int rotationOffset) const;

		grid::Grid2D getHardDropPos(const board::Board& curBoard) const;

		grid::Grid2D m_gridPos{0, 0};

		DASState m_DASState{};
		TimePoint m_lastGravityTick{Clock::now()};
		TimePoint m_lockStart{Clock::now()};

		pieceType::PieceType m_type{pieceType::PieceType::none};
		int m_rotationState{0};
	};

}
