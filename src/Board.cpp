#include "Board.hpp"

#include "Options.hpp"
using namespace options;

#include "Cell.hpp"
#include "PieceType.hpp"
#include "Pieces.hpp"
#include "PieceOffsets.hpp"

#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <array>

namespace board
{	
	Board::Board(std::size_t rows, std::size_t bufferRows, std::size_t cols, float scale, Vector3 cubeSize, grid::Grid2D gridSpawn, Vector3 position)
		: m_rows{rows},
		m_bufferRows{bufferRows},
		m_cols{cols},
		m_scale{scale},
		m_cubeSize{Vector3Scale(cubeSize, m_scale)},
		m_gridSpawn{gridSpawn.x, gridSpawn.y + static_cast<int>(bufferRows)},
		m_position{position},
		m_grid(m_rows + m_bufferRows, std::vector<cell::Cell>(m_cols))
	{
		assert(rows > 0);
		assert(cols > 0);
		assert(bufferRows > 0);
	}

	Vector3 Board::boardToWorld(grid::Grid2D pos) const
	{
		return {m_gridOrigin.x + (static_cast<float>(pos.x) * m_cubeSize.x + m_position.x),
			m_gridOrigin.y + m_position.y,
			m_gridOrigin.z + (static_cast<float>(pos.y) * m_cubeSize.z) - (static_cast<float>(m_bufferRows) * m_cubeSize.z) + m_position.z};
	}

	void Board::placePiece(const piece::Piece& curPiece)
	{
		assert(curPiece.type() != pieceType::PieceType::none);
		assert(static_cast<int>(curPiece.type()) <= 6);
		assert(curPiece.rotationState() >= 0 && curPiece.rotationState() <= 3);

		auto pieceIndex{static_cast<std::size_t>(curPiece.type())};
		auto rotationState{static_cast<std::size_t>(curPiece.rotationState())};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPos{offset + curPiece.gridPos()};

			if (!isOutOfBounds(gridPos))
			{
				if (m_grid[static_cast<std::size_t>(gridPos.y)][static_cast<std::size_t>(gridPos.x)].type == pieceType::PieceType::none)
				{
					m_grid[static_cast<std::size_t>(gridPos.y)][static_cast<std::size_t>(gridPos.x)].type = curPiece.type();
				}
			}
		}
	}

	bool Board::isColliding(grid::Grid2D testPos) const
	{
		if (testPos.x >= 0 &&
			testPos.x < static_cast<int>(m_cols) &&
			testPos.y >= 0 &&
			testPos.y < static_cast<int>(m_rows + m_bufferRows))
		{
			if (m_grid[static_cast<std::size_t>(testPos.y)][static_cast<std::size_t>(testPos.x)].type != pieceType::PieceType::none)
			{
				return true;
			}
		}
		return false;
	}

	bool Board::isOutOfBounds(grid::Grid2D testPos) const
	{
		if (testPos.x >= 0 &&
			testPos.x < static_cast<int>(m_cols) &&
			testPos.y >= 0 &&
			testPos.y < static_cast<int>(m_rows + m_bufferRows))
		{
			return false;
		}
		return true;
	}

	void Board::draw() const
	{
		for (std::size_t y{}; y < m_grid.size(); ++y)
		{
			for (std::size_t x{}; x < m_grid[0].size(); ++x)
			{
				if (m_grid[y][x].type != pieceType::PieceType::none)
				{
					std::size_t pieceIndex{static_cast<std::size_t>(m_grid[y][x].type)};

					Color mainColor{colors::piece[pieceIndex]};
					Color borderColor{colors::pieceBorder[pieceIndex]};

					Vector3 position = {boardToWorld({static_cast<int>(x), static_cast<int>(y)})};

					DrawCube(position, m_cubeSize.x, m_cubeSize.y, m_cubeSize.z, mainColor);
					DrawCubeWires(position, m_cubeSize.x, m_cubeSize.y, m_cubeSize.z, borderColor);
				}
			}
		}
	}

	void Board::drawBackground(bool drawBuffer) const
	{
		// Buffer Grid
		if (drawBuffer)
		{
			for (std::size_t i{}; i <= m_bufferRows; ++i)
			{
				float zPos{m_cubeSize.z * static_cast<float>(i)};

				Vector3 startPos{m_halfWidth, 0.0f, zPos - m_fullHeight - m_halfHeight};
				Vector3 endPos{-m_halfWidth, 0.0f, zPos - m_fullHeight - m_halfHeight};

				DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
			}

			for (int i{}; i <= m_cols; ++i)
			{
				float xPos{m_cubeSize.x * static_cast<float>(i)};

				Vector3 startPos{xPos - m_halfWidth, 0.0f, -m_fullHeight + m_halfHeight};
				Vector3 endPos{xPos - m_halfWidth, 0.0f, -m_fullHeight - m_halfHeight};

				DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
			}
		}

		// Play Grid
		for (std::size_t i{}; i <= m_rows; ++i)
		{
			float zPos{m_cubeSize.z * static_cast<float>(i)};

			Vector3 startPos{m_halfWidth, 0.0f, zPos - m_halfHeight};
			Vector3 endPos{-m_halfWidth, 0.0f, zPos - m_halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}

		for (std::size_t i{}; i <= m_cols; ++i)
		{
			float xPos{m_cubeSize.x * static_cast<float>(i)};

			Vector3 startPos{xPos - m_halfWidth, 0.0f, m_halfHeight};
			Vector3 endPos{xPos - m_halfWidth, 0.0f, -m_halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}
	}
}


