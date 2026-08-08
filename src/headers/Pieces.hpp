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

namespace board
{
	class Board;
}

namespace piece
{		
	class Piece
	{
	public:
		struct HoldState
		{
			std::chrono::time_point<std::chrono::steady_clock> lastPress;
			std::chrono::time_point<std::chrono::steady_clock> lastMove;
		};

		Piece(grid::Grid2D spawnPos, pieceType::PieceType type, int rotationState = 0);

		pieceType::PieceType type() const { return m_type; }
		int rotationState() const { return m_rotationState; }
		grid::Grid2D gridPos() const { return m_gridPos; }

		void update(const input::PieceActions& actions, board::Board& curBoard, bag::Bag& currentBag, bag::Bag& nextBag);
		void draw(const board::Board& curBoard) const;
		void drawGhostPiece(const board::Board& curBoard) const;
		
		void reset(bag::Bag& currentBag, bag::Bag& nextBag, const board::Board& curBoard);

		std::chrono::time_point<std::chrono::steady_clock> lockStart() const { return m_lockStart; } 

		bool isPositionValid(const board::Board& curBoard, grid::Grid2D testOffset = {}) const;

	private:
		std::optional<grid::Grid2D> testWallkick(wallKick::Notation notation, const board::Board& curBoard) const; 
		wallKick::Notation getWallkickNotation(int offset) const;

		grid::Grid2D getHardDropPos(const board::Board& curBoard) const;

		HoldState m_leftState{};
		HoldState m_rightState{};

		grid::Grid2D m_gridPos{0, 0};

		std::chrono::time_point<std::chrono::steady_clock> m_lastGravityTick{std::chrono::steady_clock::now()};
		std::chrono::time_point<std::chrono::steady_clock> m_lockStart{std::chrono::steady_clock::now()};

		pieceType::PieceType m_type{};
		int m_rotationState{0};

	};
}
