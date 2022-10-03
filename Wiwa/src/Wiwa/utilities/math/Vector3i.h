#pragma once

namespace Wiwa {
	struct Vector3i {
		union
		{
			struct { int x, y, z; };
		};
	};

	typedef Vector3i Size3i;
	typedef Vector3i Color3i;
}