#pragma once

#include "Empathy/Core.h"
#include "Empathy/headers/math/Vector2f.h"

#include "Empathy/headers/Reflection.h"

namespace EM {
	struct EM_API Transform2D {
		Vector2i position;
		float rotation;
		Vector2f scale;
	};
}

REFLECTION_BEGIN(EM::Transform2D);
	REFLECT_MEMBER(position);
	REFLECT_MEMBER(rotation);
	REFLECT_MEMBER(scale);
REFLECTION_END;