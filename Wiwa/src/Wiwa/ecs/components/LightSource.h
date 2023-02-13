#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/Reflection.h>


namespace Wiwa {
	struct WI_API LightSource {
		float constant;
		float linear;
		float quadratic;

		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
	};
}

REFLECTION_BEGIN(Wiwa::LightSource)
	REFLECT_MEMBER(constant)
	REFLECT_MEMBER(linear)
	REFLECT_MEMBER(quadratic)
	REFLECT_MEMBER(ambient)
	REFLECT_MEMBER(diffuse)
	REFLECT_MEMBER(specular)
REFLECTION_END;