#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/Reflection.h>
#include <Wiwa/Resources.h>

namespace Wiwa {
	struct WI_API Mesh {
		ResourceId meshId;
		ResourceId materialId;
	};
}

REFLECTION_BEGIN(Wiwa::Mesh)
REFLECT_MEMBER(meshId)
REFLECT_MEMBER(materialId)
REFLECTION_END;
