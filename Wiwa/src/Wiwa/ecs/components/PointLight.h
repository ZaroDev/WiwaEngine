#pragma once
#pragma warning(disable : 4302)
#include <Wiwa/core/Core.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/Reflection.h>


namespace Wiwa {
	struct WI_API PointLight {
		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}

REFLECTION_BEGIN(Wiwa::PointLight)
	REFLECT_MEMBER(constant)
	REFLECT_MEMBER(linear)
	REFLECT_MEMBER(quadratic)
	REFLECT_MEMBER(ambient)
	REFLECT_MEMBER(diffuse)
	REFLECT_MEMBER(specular)
REFLECTION_END;