#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

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
