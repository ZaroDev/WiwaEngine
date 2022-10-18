#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/Reflection.h>
#include <Wiwa/Resources.h>

namespace Wiwa {
	struct WI_API Mesh {
		ResourceId id;
	};
}

REFLECTION_BEGIN(Wiwa::Mesh)
REFLECT_MEMBER(id)
REFLECTION_END;
