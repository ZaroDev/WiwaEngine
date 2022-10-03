#pragma once

namespace Wiwa {
	struct Vector4i {
		union
		{
			struct { int x, y, z, w; };
			struct { int r, g, b, a; };
		};
	};

	typedef Vector4i Size4i;
	typedef Vector4i Color4i;
}