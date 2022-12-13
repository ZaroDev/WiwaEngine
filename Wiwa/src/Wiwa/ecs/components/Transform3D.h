#pragma once
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Transform3D {
		// Read-only world position
		Vector3f position;
		// Read-only world rotation
		Vector3f rotation;
		// Read-only world scale
		Vector3f scale;

		// Local position
		Vector3f localPosition;
		// Local rotation
		Vector3f localRotation;
		// Local scale
		Vector3f localScale;
	};
}

REFLECTION_BEGIN(Wiwa::Transform3D)
	REFLECT_MEMBER(position)
	REFLECT_MEMBER(rotation)
	REFLECT_MEMBER(scale)
	REFLECT_MEMBER(localPosition)
	REFLECT_MEMBER(localRotation)
	REFLECT_MEMBER(localScale)
REFLECTION_END;