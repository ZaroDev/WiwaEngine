#pragma once

#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Vector3f.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	struct WI_API AudioSource {
		uint32_t radius;
		uint32_t volume;
		Vector3f direction;
		bool posted;
	};
}

REFLECTION_BEGIN(Wiwa::AudioSource)
	REFLECT_MEMBER(radius)
	REFLECT_MEMBER(volume)
	REFLECT_MEMBER(direction)
REFLECTION_END;