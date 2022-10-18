#pragma once

#include <Wiwa/Core.h>
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/Resources.h>
#include <Wiwa/Renderer2D.h>

#include <Wiwa/Reflection.h>

namespace Wiwa {
	struct WI_API Sprite {
		Size2i sizeScreen;
		ResourceId spriteResource;
		Rect2i rect;
		Renderer2D::Pivot pivot;
	};
}

REFLECTION_BEGIN(Wiwa::Sprite);
	REFLECT_MEMBER(sizeScreen);
	REFLECT_MEMBER(spriteResource);
	REFLECT_MEMBER(rect);
	REFLECT_MEMBER(pivot);
REFLECTION_END;