#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Transform2D {
		Vector2i position;
		float rotation;
		Vector2f scale;
	};
}

REFLECTION_BEGIN(Wiwa::Transform2D)
	REFLECT_MEMBER(position)
	REFLECT_MEMBER(rotation)
	REFLECT_MEMBER(scale)
REFLECTION_END;