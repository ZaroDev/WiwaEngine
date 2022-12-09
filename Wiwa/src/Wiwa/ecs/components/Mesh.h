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
		bool drawChildren;
		size_t modelIndex;
		char mesh_path[128];
		char mat_path[128];
	};
}

REFLECTION_BEGIN(Wiwa::Mesh)
	REFLECT_MEMBER(meshId)
	REFLECT_MEMBER(materialId)
	REFLECT_MEMBER(drawChildren)
	REFLECT_MEMBER(modelIndex)
	REFLECT_MEMBER(mesh_path)
	REFLECT_MEMBER(mat_path)
REFLECTION_END;