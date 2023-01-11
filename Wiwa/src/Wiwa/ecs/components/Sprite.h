#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/core/Resources.h>


#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API Sprite {
		Size2i sizeScreen;
		ResourceId spriteResource;
		Rect2i rect;
		
	};
}

REFLECTION_BEGIN(Wiwa::Sprite);
	REFLECT_MEMBER(sizeScreen);
	REFLECT_MEMBER(spriteResource);
	REFLECT_MEMBER(rect);

REFLECTION_END;