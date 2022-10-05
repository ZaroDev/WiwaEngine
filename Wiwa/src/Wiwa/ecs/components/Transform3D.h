#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>

namespace Wiwa {
	struct WI_API Transform3D {
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};
}