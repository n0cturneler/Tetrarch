#include "Board.hpp"

#include "Options.hpp"
using namespace options;

#include "Cell.hpp"
#include "PieceType.hpp"
#include "Pieces.hpp"
#include "PieceOffsets.hpp"
#include "Random.hpp"

#include <raylib.h>
#include <raymath.h>

#include <cassert>
#include <vector>
#include <array>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace board
{
	Board::Board(std::size_t rows, std::size_t bufferRows, std::size_t cols, float scale, Vector3 cubeSize, grid::Grid2D gridSpawn, Vector3 position)
		: m_currentBag{Random::mt},
		m_nextBag{Random::mt},
		m_rows{rows},
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

		m_startTime = Clock::now();
	}

	Vector3 Board::boardToWorld(grid::Grid2D pos) const
	{
		return {
			m_gridOrigin.x + (static_cast<float>(pos.x) * m_cubeSize.x) + m_position.x,
			m_gridOrigin.y + m_position.y,
			m_gridOrigin.z + (static_cast<float>(pos.y) * m_cubeSize.z) - (static_cast<float>(m_bufferRows) * m_cubeSize.z) + m_position.z
		};
	}

	void Board::updatePosition(const input::PieceActions& actions)
	{	
		constexpr float maxZ{1.0f};
		constexpr float maxX{0.5f};

		if (actions.hardDrop)
		{
			m_position.z = std::min(m_position.z += 0.25f, maxZ);
		}

		if (actions.holdLeft)
		{
			m_position.x = Lerp(m_position.x, m_position.x - 0.5f, GetFrameTime());
		}

		if (actions.holdRight)
		{
			m_position.x = Lerp(m_position.x, m_position.x + 0.5f, GetFrameTime());
		}

		const float intensity{m_position.z / maxZ};
		m_position.x += std::sin(GetTime() * 5.0f) * (0.05f * intensity);

		m_position.x = std::clamp(m_position.x, -maxX, maxX);

		m_position.x = Lerp(m_position.x, 0.0f, GetFrameTime() * 5.0f);
		m_position.z = Lerp(m_position.z, 0.0f, GetFrameTime() * 10.0f);
	}

	void Board::placePiece(const piece::Piece& curPiece)
	{
		m_canHold = true;
		m_pieceCount += 1;

		assert(curPiece.type() != pieceType::PieceType::none);
		assert(static_cast<int>(curPiece.type()) <= 6);
		assert(curPiece.rotationState() >= 0 && curPiece.rotationState() <= 3);

		auto pieceIndex{static_cast<std::size_t>(curPiece.type())};
		auto rotationState{static_cast<std::size_t>(curPiece.rotationState())};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		std::vector<std::size_t> placedRows;
		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPos{offset + curPiece.gridPos()};

			if (!isOutOfBounds(gridPos))
			{
				if (m_grid[static_cast<std::size_t>(gridPos.y)][static_cast<std::size_t>(gridPos.x)].type == pieceType::PieceType::none)
				{
					placedRows.push_back(static_cast<std::size_t>(gridPos.y));
					m_grid[static_cast<std::size_t>(gridPos.y)][static_cast<std::size_t>(gridPos.x)].type = curPiece.type();
				}
			}
		}
		std::sort(placedRows.begin(), placedRows.end());
		clearLines(placedRows);

		isGameOver();
	}

	void Board::clearLines(const std::vector<std::size_t>& placedRows)
	{
		int linesCleared{};
		for (std::size_t i : placedRows)
		{
			bool full{true};

			for (const cell::Cell& cell : m_grid[i])
			{
				if (cell.type == pieceType::PieceType::none)
				{
					full = false;
					break;
				}
			}

			if (full)
			{	
				linesCleared += 1;
				for (cell::Cell& cell : m_grid[i])
				{
					cell.type = pieceType::PieceType::none;
				}
			}
		}

		std::size_t writeY{m_grid.size() - 1};

		for (std::size_t i{m_grid.size()}; i-- > 0;)
		{
			bool empty{true};
			for (const cell::Cell& cell : m_grid[i])
			{
				if (cell.type != pieceType::PieceType::none)
				{
					empty = false;
					break;
				}
			}
			if (!empty)
			{
				if (writeY != i)
				{
					m_grid[writeY] = m_grid[i];
				}
				writeY -= 1;
			}
		}

		for (std::size_t i{}; i <= writeY; ++i)
		{
			for (cell::Cell& cell : m_grid[i])
			{
				cell.type = pieceType::PieceType::none;
			}
		}
	}

	void Board::isGameOver() 
	{
		const std::size_t pieceIndex{static_cast<std::size_t>(m_currentBag.peek(m_currentBag.currentIndex()))};
		const std::size_t rotationState{0};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		Color mainColor{Fade(colors::textGray, 0.5f)};

		for (const grid::Grid2D& offset : data)
		{	
			grid::Grid2D gridPosition = {m_gridSpawn + offset};
			
			if (m_grid[gridPosition.y][gridPosition.x].type != pieceType::PieceType::none)
			{
				m_alive = false;
				break;
			}
		}
	}

	pieceType::PieceType Board::getNextPieceType()
	{
		return m_currentBag.getNextpieceType(m_nextBag);
	}

	pieceType::PieceType Board::holdPiece(const piece::Piece& curPiece)
	{
		if (!m_canHold) { return curPiece.type(); }

		m_canHold = false;
		pieceType::PieceType heldType{m_heldPiece};
		m_heldPiece = curPiece.type();

		if (heldType != pieceType::PieceType::none)	return heldType;
		return getNextPieceType();
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
				float zPos{m_cubeSize.z * static_cast<float>(i) + m_position.z};

				Vector3 startPos{m_halfWidth + m_position.x, 0.0f, zPos - m_fullHeight - m_halfHeight};
				Vector3 endPos{-m_halfWidth + m_position.x, 0.0f, zPos - m_fullHeight - m_halfHeight};

				DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
			}

			for (std::size_t i{}; i <= m_cols; ++i)
			{
				float xPos{m_cubeSize.x * static_cast<float>(i) + m_position.x};

				Vector3 startPos{xPos - m_halfWidth, 0.0f, m_halfHeight + m_position.z - m_fullHeight};
				Vector3 endPos{xPos - m_halfWidth, 0.0f, -m_halfHeight + m_position.z - m_fullHeight};

				DrawLine3D(startPos, endPos, colors::backgroundBufferLines);
			}
		}

		// Play Grid
		for (std::size_t i{}; i <= m_rows; ++i)
		{
			float zPos{m_cubeSize.z * static_cast<float>(i) + m_position.z};

			Vector3 startPos{m_halfWidth + m_position.x, 0.0f, zPos - m_halfHeight};
			Vector3 endPos{-m_halfWidth + m_position.x, 0.0f, zPos - m_halfHeight};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}

		for (std::size_t i{}; i <= m_cols; ++i)
		{
			float xPos{m_cubeSize.x * static_cast<float>(i) + m_position.x};

			Vector3 startPos{xPos - m_halfWidth, 0.0f, m_halfHeight + m_position.z};
			Vector3 endPos{xPos - m_halfWidth, 0.0f, -m_halfHeight + m_position.z};

			DrawLine3D(startPos, endPos, colors::backgroundLines);
		}
	}

	void Board::drawSpawnLocation() const
	{
		const std::size_t pieceIndex{static_cast<std::size_t>(m_currentBag.peek(m_currentBag.currentIndex()))};
		const std::size_t rotationState{0};
		const auto& data{pieceData::Data[pieceIndex][rotationState]};

		Color mainColor{Fade(colors::textGray, 0.5f)};

		for (const grid::Grid2D& offset : data)
		{
			grid::Grid2D gridPosition = {m_gridSpawn + offset};
			Vector3 position = {boardToWorld(gridPosition)};

			DrawCube(position, m_cubeSize.x / 2, m_cubeSize.y / 2, m_cubeSize.z / 2, mainColor);
			DrawCubeWires(position, m_cubeSize.x, m_cubeSize.y, m_cubeSize.z, mainColor);
		}
	}
}

