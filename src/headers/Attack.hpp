#pragma once

#include <array>

namespace attack
{
	enum class ClearType
	{
		Single,
		Double,
		Triple,
		Tetris,

		SpinSingle,
		SpinDouble,
		SpinTriple
	};

	inline constexpr std::array<int, 7> attackValue
	{
		0,
		1,
		2,
		4,
		2,
		4,
		6
	};

	inline constexpr int getAttackValue(ClearType type)
	{
		return attackValue[static_cast<std::size_t>(type)];
	}
}
