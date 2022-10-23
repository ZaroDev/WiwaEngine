#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
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

REFLECTION_BEGIN(Wiwa::Transform3D)
	REFLECT_MEMBER(position)
	REFLECT_MEMBER(rotation)
	REFLECT_MEMBER(scale)
REFLECTION_END;