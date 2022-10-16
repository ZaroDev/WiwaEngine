#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/Reflection.h>

namespace Wiwa {
	struct WI_API Transform3D {
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};
}

CORE_REFLECTION_BEGIN(Wiwa::Transform3D)
	CORE_REFLECT_MEMBER(position)
	CORE_REFLECT_MEMBER(rotation)
	CORE_REFLECT_MEMBER(scale)
CORE_REFLECTION_END;