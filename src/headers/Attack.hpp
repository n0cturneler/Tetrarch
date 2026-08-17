#pragma once

#include <array>
#include <iostream>
#include <string_view>

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

	inline constexpr ClearType getClearType(int rawAttack)
	{
		switch (rawAttack)
		{
		case 0: return ClearType::Single;
		case 1: return ClearType::Single;
		case 2: return ClearType::Double;
		case 3: return ClearType::Triple;
		case 4: return ClearType::Tetris;
		default: break;
		}
	}

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

	inline constexpr std::string_view getAttackString(ClearType type)
	{
		switch (type)
		{
		case ClearType::Single: return "SINGLE";
		case ClearType::Double: return "DOUBLE";
		case ClearType::Triple: return "TRIPLE";
		case ClearType::Tetris: return "TETRIS";

		case ClearType::SpinSingle: return "-SPIN SINGLE";
		case ClearType::SpinDouble: return "-SPIN DOUBLE";
		case ClearType::SpinTriple: return "-SPIN TRIPLE";

		default: break;
		}
	}
}
