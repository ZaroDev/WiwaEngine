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

CORE_REFLECTION_BEGIN(Wiwa::Sprite);
	CORE_REFLECT_MEMBER(sizeScreen);
	CORE_REFLECT_MEMBER(spriteResource);
	CORE_REFLECT_MEMBER(rect);
	CORE_REFLECT_MEMBER(pivot);
CORE_REFLECTION_END;