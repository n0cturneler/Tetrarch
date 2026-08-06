#pragma once

#include <iostream>
#include <raylib.h>

namespace grid
{
	struct Grid2D
	{
		int x{};
		int y{};

		friend bool operator==(const Grid2D& lhs, const Grid2D& rhs)
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y);
		}
		friend bool operator!=(const Grid2D& lhs, const Grid2D& rhs)
		{
			return !(lhs == rhs);
		}
	};

	Vector3 gridToWorld(Grid2D gridPos);

	Grid2D add(Grid2D grid1, Grid2D grid2);
	Grid2D inverse(Grid2D grid);
}
