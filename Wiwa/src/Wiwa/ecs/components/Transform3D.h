#pragma once

#include "Empathy/Core.h"
#include "Empathy/headers/math/Vector3f.h"

namespace EM {
	struct EM_API Transform3D {
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;
	};
}