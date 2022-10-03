#pragma once

namespace Wiwa {
	struct Vector2i {
		union
		{
			struct { int x, y; };
			struct { int w, h; };
		};
	};

	inline Vector2i operator/(const Vector2i& first, int second) { return { first.x / second, first.y / second }; };
	inline Vector2i operator+(Vector2i& v1, Vector2i& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
	inline Vector2i operator+(Vector2i v1, Vector2i v2) { return { v1.x + v2.x, v1.y + v2.y }; }

	typedef Vector2i Size2i;
}