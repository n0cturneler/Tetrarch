#include "Bag.hpp"

#include "Random.hpp"

#include <array>
#include <algorithm>
#include <cassert>

namespace bag
{
	Bag::Bag(std::mt19937& rng)
	{
		std::shuffle(m_data.begin(), m_data.end(), rng);
	}

	pieceType::PieceType Bag::getNextpieceType(Bag& nextBag)
	{
		assert(m_currentIndex < 7);

		pieceType::PieceType type{};

		if (m_currentIndex < 7)
		{
			type = m_data[m_currentIndex];
		}

		m_currentIndex += 1;

		if (m_currentIndex >= 7)
		{
			m_data = nextBag.m_data;
			nextBag.reshuffle(Random::mt);
			m_currentIndex = 0;
		}

		return type;
	}

	void Bag::reshuffle(std::mt19937& rng)
	{
		std::shuffle(m_data.begin(), m_data.end(), rng);
	}

}
