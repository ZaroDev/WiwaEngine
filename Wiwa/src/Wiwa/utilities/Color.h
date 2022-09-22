#pragma once

#include "Empathy/headers/math/Vector4f.h"

namespace EM {
	struct Color {
		int red, green, blue, alpha;
		float r, g, b, a;

		inline int ToByte32() {
			return (red << 24) + (green << 16) + (blue << 8) + alpha;
		}
		inline void ToFloat() {
			r = red / 255.0f; g = green / 255.0f; b = blue / 255.0f; a = alpha / 255.0f;
		}
	};

	namespace EColor {
		const Color4f WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Color4f BLACK = { 0.0f, 0.0f, 0.0f, 0.0f };

		const Color4f RED = { 1.0f, 0.0f, 0.0f, 1.0f };
		const Color4f GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
		const Color4f BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };

		const Color4f YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	}
}