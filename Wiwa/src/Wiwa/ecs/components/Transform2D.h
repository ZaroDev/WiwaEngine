#pragma once

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

REFLECTION_BEGIN(Wiwa::Transform2D);
	REFLECT_MEMBER(position);
	REFLECT_MEMBER(rotation);
	REFLECT_MEMBER(scale);
REFLECTION_END;