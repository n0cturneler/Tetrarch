#pragma once

#include <iostream>
#include <raylib.h>

namespace grid
{
	struct Grid2D
	{
		int x{};
		int y{};

		friend Grid2D operator+(const Grid2D& lhs, const Grid2D& rhs)
		{
			return {lhs.x + rhs.x, lhs.y + rhs.y};
		}

		friend Grid2D operator-(const Grid2D& lhs, const Grid2D& rhs)
		{
			return {lhs.x - rhs.x, lhs.y - rhs.y};
		}

		friend Grid2D operator-(const Grid2D& lhs)
		{
			return {-lhs.x, - lhs.y};
		}

		friend bool operator==(const Grid2D& lhs, const Grid2D& rhs)
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y);
		}

		friend bool operator!=(const Grid2D& lhs, const Grid2D& rhs)
		{
			return !(lhs == rhs);
		}
	};
}
