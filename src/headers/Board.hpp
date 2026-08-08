#pragma once

#include "Cell.hpp"
#include "Pieces.hpp"

#include <vector>

namespace piece
{
	class Piece;
}

namespace board
{	
	class Board
	{
	public:
		Board(std::size_t rows, std::size_t bufferRows, std::size_t cols);

		grid::Grid2D spawnPos() const { return m_spawnPos; }
		std::size_t bufferRows() const { return m_bufferRows; }
		std::size_t fullRows() const { return m_rows + m_bufferRows; }

		Vector3 boardToWorld(grid::Grid2D pos) const;

		void placePiece(const piece::Piece& curPiece);

		bool isColliding(grid::Grid2D testPos) const;
		bool isOutOfBounds(grid::Grid2D testPos) const;

		void draw() const;

	private:
		std::size_t m_rows{};
		std::size_t m_bufferRows{};
		std::size_t m_cols{};

		grid::Grid2D m_spawnPos{0,0};

		std::vector<std::vector<cell::Cell>> m_grid;
	};
}
