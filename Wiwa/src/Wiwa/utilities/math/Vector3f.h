#pragma once

namespace Wiwa {
	struct Vector3f {
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
			struct { float s, t, p; };
		};
	};

	typedef Vector3f Size3f;
	typedef Vector3f Color3f;

	inline Vector3f operator-(const Vector3f v1, const Vector3f v2) {
		return { v1.x - v2.x, v1.y - v2.y };
	}
}