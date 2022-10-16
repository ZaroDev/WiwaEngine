#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/Reflection.h>

namespace Wiwa {
	struct WI_API Transform2D {
		Vector2i position;
		float rotation;
		Vector2f scale;
	};
}

CORE_REFLECTION_BEGIN(Wiwa::Transform2D)
	CORE_REFLECT_MEMBER(position)
	CORE_REFLECT_MEMBER(rotation)
	CORE_REFLECT_MEMBER(scale)
CORE_REFLECTION_END;