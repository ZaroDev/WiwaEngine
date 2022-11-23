#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>
#include <Wiwa/core/Resources.h>

namespace Wiwa {
	struct WI_API Mesh {
		char mesh_path[128];
		ResourceId meshId;
		char mat_path[128];
		ResourceId materialId;
	};
}

REFLECTION_BEGIN(Wiwa::Mesh)
	REFLECT_MEMBER(mesh_path)
	REFLECT_MEMBER(meshId)
	REFLECT_MEMBER(mat_path)
	REFLECT_MEMBER(materialId)
REFLECTION_END;