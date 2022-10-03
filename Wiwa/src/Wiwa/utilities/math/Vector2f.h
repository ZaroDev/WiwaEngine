#pragma once

namespace Wiwa {
	struct Vector2f {
		union
		{
			struct { float x, y; };
			struct { float w, h; };
		};
	};

	inline void operator*=(Vector2f& origin, float value) { origin.x *= value; origin.y *= value; }
	inline void operator-=(Vector2f& origin, float value) { origin.x -= value; origin.y -= value; }

	inline Vector2f operator+=(Vector2f& v1, Vector2f& v2) { v1.x += v2.x; v1.y += v2.y; }

	inline Vector2f operator+(Vector2f& v1, Vector2f& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	inline Vector2f operator+(Vector2f v1, Vector2f v2) { return { v1.x + v2.x, v1.y + v2.y }; }

	typedef Vector2f Size2f;
}