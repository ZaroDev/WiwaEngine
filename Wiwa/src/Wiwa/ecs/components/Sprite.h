#pragma once

#include "Empathy/Core.h"
#include "Empathy/headers/math/Vector2f.h"
#include "Empathy/headers/math/Rectangle.h"
#include "Empathy/headers/Resources.h"
#include "Empathy/headers/Renderer2D.h"

#include "Empathy/headers/Reflection.h"

namespace EM {
	struct EM_API Sprite {
		Size2i sizeScreen;
		ResourceId spriteResource;
		Rectangle rect;
		Renderer2D::Pivot pivot;
	};
}

REFLECTION_BEGIN(EM::Sprite);
	REFLECT_MEMBER(sizeScreen);
	REFLECT_MEMBER(spriteResource);
	REFLECT_MEMBER(rect);
	REFLECT_MEMBER(pivot);
REFLECTION_END;