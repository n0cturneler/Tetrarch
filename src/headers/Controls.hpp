#pragma once

#include <raylib.h>
namespace controls
{
	using Key = KeyboardKey;

	// MOVEMENT
	inline constexpr Key moveLeft{KEY_LEFT};
	inline constexpr Key moveRight{KEY_RIGHT};

	// ROTATION
	inline constexpr Key rotLeft{KEY_Z};
	inline constexpr Key rotRight{KEY_X};
	inline constexpr Key rot180{KEY_C};

	// PLACE
	inline constexpr Key hardDrop{KEY_SPACE};
	inline constexpr Key softDrop{KEY_DOWN};

	// HOLD
	inline constexpr Key hold{KEY_LEFT_SHIFT};

}
