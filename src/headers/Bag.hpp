#pragma once

#include "PieceType.hpp"

#include <raylib.h>

#include <array>
#include <random>
#include <optional>

namespace bag
{
	class Bag
	{
	public:
		Bag(std::mt19937& rng);


		pieceType::PieceType getNextpieceType(bag::Bag& nextBag);

		const std::array<pieceType::PieceType, 7>& data() const { return m_data; }
		std::size_t currentIndex() const { return m_currentIndex; }
		pieceType::PieceType peek(std::size_t i) const { return (i < m_data.size()) ? m_data[i] : pieceType::PieceType::none; }

	private:
		void reshuffle(std::mt19937& rng);
		
		std::array<pieceType::PieceType, 7> m_data{pieceType::PieceType::I, 
			pieceType::PieceType::J, 
			pieceType::PieceType::L, 
			pieceType::PieceType::O, 
			pieceType::PieceType::S, 
			pieceType::PieceType::T, 
			pieceType::PieceType::Z};

		std::size_t m_currentIndex{0};

		pieceType::PieceType heldPiece{pieceType::PieceType::none};
		bool canHold{};
	};

}
