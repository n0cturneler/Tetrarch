#pragma once

#include "Grid.hpp"
#include <array>

namespace wallKick
{
	using Grid2D = grid::Grid2D;
	using Test = std::array<Grid2D, 5>;
	using Data = std::array<Test, 8>;

	enum class Notation
	{
		Zero_to_R,
		R_to_Zero,
		R_to_Two,
		Two_to_R,
		Two_to_L,
		L_to_Two,
		L_to_Zero,
		Zero_to_L,
		Invalid
	};

	inline constexpr Grid2D SRStoGrid(grid::Grid2D SRSpos)
	{
		return {SRSpos.x, SRSpos.y * -1};
	}

	// Original SRS wallkick data
	// SRStoGrid() inverts the y axis basically y * -1 
	// 
	// Its in this order
	// 0->R
	// R->0
	// R->2
	// 2->R
	// L->2
	// L->0
	// 0->L

	inline constexpr Data JLSTZ
	{
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({-1, 1}),
			SRStoGrid({0, -2}),
			SRStoGrid({-1, -2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({1, -1}),
			SRStoGrid({0, 2}),
			SRStoGrid({1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({1, -1}),
			SRStoGrid({0, 2}),
			SRStoGrid({1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({-1, 1}),
			SRStoGrid({0, -2}),
			SRStoGrid({-1, -2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({1, 1}),
			SRStoGrid({0, -2}),
			SRStoGrid({1, -2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({-1, -1}),
			SRStoGrid({0, 2}),
			SRStoGrid({-1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({-1, -1}),
			SRStoGrid({0, 2}),
			SRStoGrid({-1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({1, 1}),
			SRStoGrid({0, -2}),
			SRStoGrid({1, -2})
	}
	};

	inline constexpr Data I
	{
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-2, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({-2, -1}),
			SRStoGrid({1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({2, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({2, 1}),
			SRStoGrid({-1, -2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({2, 0}),
			SRStoGrid({-1, 2}),
			SRStoGrid({2, -1})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({-2, 0}),
			SRStoGrid({1, -2}),
			SRStoGrid({-2, 1})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({2, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({2, 1}),
			SRStoGrid({-1, -2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-2, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({-2, -1}),
			SRStoGrid({1, 2})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({1, 0}),
			SRStoGrid({-2, 0}),
			SRStoGrid({1, -2}),
			SRStoGrid({-2, 1})
	},
		Test{
			SRStoGrid({0, 0}),
			SRStoGrid({-1, 0}),
			SRStoGrid({2, 0}),
			SRStoGrid({-1, 2}),
			SRStoGrid({2, -1})
	}
	};
}
