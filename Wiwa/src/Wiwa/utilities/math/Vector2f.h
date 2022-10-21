#pragma once

namespace Wiwa {
	struct Vector2f {
		union
		{
			struct { float x, y; };
			struct { float w, h; };
		};

		inline static Vector2f Zero() { return { 0.0f, 0.0f }; }
	};

	inline void operator*=(Vector2f& origin, float value) { origin.x *= value; origin.y *= value; }
	inline void operator-=(Vector2f& origin, float value) { origin.x -= value; origin.y -= value; }

	inline Vector2f operator+=(Vector2f& v1, Vector2f& v2) { v1.x += v2.x; v1.y += v2.y; }

	inline Vector2f operator+(Vector2f& v1, Vector2f& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	inline Vector2f operator+(Vector2f v1, Vector2f v2) { return { v1.x + v2.x, v1.y + v2.y }; }

	inline Vector2f operator-(Vector2f& v1, Vector2f& v2) { return { v1.x - v2.x, v1.y - v2.y }; }

	inline bool operator==(Vector2f v1, Vector2f v2) { return v1.x == v2.x && v1.y == v2.y; }
	inline bool operator!=(Vector2f v1, Vector2f v2) { return !(v1 == v2); }

	typedef Vector2f Size2f;
}