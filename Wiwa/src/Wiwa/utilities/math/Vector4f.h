#pragma once

#include <Wiwa/Reflection.h>

namespace Wiwa {
	struct Vector4f {
		union
		{
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};
	};

	typedef Vector4f Size4f;
	typedef Vector4f Color4f;

	namespace Color {
		const Color4f WHITE = { 1.0f, 1.0f, 1.0f };
	}
}

REFLECTION_BEGIN(Wiwa::Vector4f)
	REFLECT_MEMBER(x)
	REFLECT_MEMBER(y)
	REFLECT_MEMBER(z)
	REFLECT_MEMBER(w)
REFLECTION_END;