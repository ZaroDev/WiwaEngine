#pragma once

namespace Wiwa {
	struct Vector3f {
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
			struct { float s, t, p; };
		};
		Vector3f() = default;
		Vector3f(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		inline Vector3f& operator+=(const Vector3f& v1) {
			
			Vector3f res = { this->x + v1.x, this->y + v1.y, this->z + v1.z };
			return res;
		}
		inline Vector3f& operator-=(const Vector3f& v1) {
			Vector3f res = { this->x - v1.x, this->y - v1.y, this->z - v1.z };
			return res;
		}
	};

	typedef Vector3f Size3f;
	typedef Vector3f Color3f;

	inline Vector3f operator-(const Vector3f v1, const Vector3f v2) {
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}
	inline Vector3f operator+(const Vector3f v1, const Vector3f v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}
	
}