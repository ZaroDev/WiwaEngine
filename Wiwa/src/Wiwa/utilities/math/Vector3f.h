#pragma once

namespace Wiwa {
	struct Vector3f {
		union
		{
			struct { float x, y, z; };
			struct { float r, g, b; };
			struct { float s, t, p; };
		};
	};

	typedef Vector3f Size3f;
	typedef Vector3f Color3f;
}