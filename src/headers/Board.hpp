#pragma once

#include "Cell.hpp"

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
		Board(std::size_t rows, std::size_t bufferRows, std::size_t cols, float scale, Vector3 cubeSize, grid::Grid2D gridSpawn = {4,0}, Vector3 position = {});

		grid::Grid2D spawnPos() const { return m_gridSpawn; }
		float scale() const { return m_scale; }
		Vector3 cubeSize() const { return m_cubeSize; }
		std::size_t bufferRows() const { return m_bufferRows; }
		std::size_t fullRows() const { return m_rows + m_bufferRows; }

		Vector3 boardToWorld(grid::Grid2D pos) const;

		void placePiece(const piece::Piece& curPiece);

		bool isColliding(grid::Grid2D testPos) const;
		bool isOutOfBounds(grid::Grid2D testPos) const;

		void draw() const;
		void drawBackground(bool drawBuffer) const;

	private:
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

		std::vector<std::vector<cell::Cell>> m_grid;
	};
}
