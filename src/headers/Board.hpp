#pragma once

#include "Cell.hpp"
#include "Bag.hpp"
#include "Input.hpp"
#include "Attack.hpp"

#include <vector>
#include <chrono>
#include <cstdint>
#include <string>

namespace piece
{
	class Piece;
}

namespace board
{	
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using MS = std::chrono::milliseconds;

	class Board
	{
	public:
		Board(std::size_t rows, std::size_t bufferRows, std::size_t cols, float scale, Vector3 cubeSize, grid::Grid2D gridSpawn = {4,1}, Vector3 position = {});

		void updatePosition(const input::PieceActions& actions);

		float scale() const { return m_scale; }
		std::uint64_t pieceCount() const { return m_pieceCount; }
		std::uint64_t attack() const { return m_attack; }
		TimePoint startTime() const { return m_startTime; }

		std::size_t bufferRows() const { return m_bufferRows; }
		std::size_t fullRows() const { return m_rows + m_bufferRows; }

		grid::Grid2D spawnPos() const { return m_gridSpawn; }

		Vector3 cubeSize() const { return m_cubeSize; }
		Vector3 boardToWorld(grid::Grid2D pos) const;

		void placePiece(const piece::Piece& curPiece);

		bool isColliding(grid::Grid2D testPos) const;
		bool isOutOfBounds(grid::Grid2D testPos) const;

		bool isAlive() const { return m_alive; }

		void draw() const;
		void drawBackground(bool drawBuffer) const;
		void drawSpawnLocation() const;

		pieceType::PieceType getNextPieceType();

		bool canHold() const { return m_canHold; }
		pieceType::PieceType holdPiece(const piece::Piece& curPiece);
		pieceType::PieceType heldPiece() const { return m_heldPiece; }

		const bag::Bag& currentBag() const { return m_currentBag; }
		const bag::Bag& nextBag() const { return m_nextBag; }

		attack::ClearType lastClearType() const { return m_lastClearType; }
	private:
		void clearLines(const std::vector<std::size_t>& curPiece);
		void isGameOver();

		bag::Bag m_currentBag;
		bag::Bag m_nextBag;

		std::size_t m_rows{};
		std::size_t m_bufferRows{};
		std::size_t m_cols{};

		float m_scale{1.0};

		Vector3 m_cubeSize{};

		grid::Grid2D m_gridSpawn{};

		Vector3 m_halfCube{m_cubeSize.x * 0.5f, m_cubeSize.y * 0.5f, m_cubeSize.z * 0.5f};
		
		float m_fullHeight{m_cubeSize.z * static_cast<float>(m_rows)};
		float m_halfHeight{m_cubeSize.z * static_cast<float>(m_rows) * 0.5f};
		float m_halfWidth{m_cubeSize.x * static_cast<float>(m_cols) * 0.5f};

		Vector3 m_position{};
		Vector3 m_gridOrigin{m_position.x + m_halfCube.x - m_halfWidth, m_halfCube.y, m_position.z - m_halfHeight + m_halfCube.z};

		pieceType::PieceType m_heldPiece{pieceType::PieceType::none};
		bool m_canHold{true};

		bool m_alive{true};

		std::uint64_t m_pieceCount{};
		std::uint64_t m_lineCleared{};
		std::uint64_t m_score{};
		std::uint64_t m_attack{};

		attack::ClearType m_lastClearType{attack::ClearType::none};

		TimePoint m_startTime{Clock::now()};

		std::vector<std::vector<cell::Cell>> m_grid;
	};
}
