#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Transform3D {
		Vector3f position;
		Vector3f localPosition;
		Vector3f rotation;
		Vector3f localRotation;
		Vector3f scale;
		Vector3f localScale;
	};
}

REFLECTION_BEGIN(Wiwa::Transform3D)
	REFLECT_MEMBER(position)
	REFLECT_MEMBER(localPosition)
	REFLECT_MEMBER(rotation)
	REFLECT_MEMBER(localRotation)
	REFLECT_MEMBER(scale)
	REFLECT_MEMBER(localScale)
REFLECTION_END;