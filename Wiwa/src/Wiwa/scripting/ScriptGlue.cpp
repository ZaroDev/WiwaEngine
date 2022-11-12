#include <wipch.h>
#include "ScriptGlue.h"

#include <mono/metadata/object.h>
#include <glm.hpp>
#include <Wiwa/ecs/systems/System.h>

namespace Wiwa {

#define WI_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Wiwa.InternalCalls::" #Name, Name)
	static void CppFunc()
	{
		std::cout << "This is written in C++" << std::endl;
	}
	static void NativeLog(MonoString* string, int parameter)
	{
		char* str = mono_string_to_utf8(string);

		WI_CORE_TRACE("{0} {1}", str, parameter);
		mono_free(str);
	}
	static void NativeLogVector(glm::vec3* parameter, glm::vec3* outParam)
	{
		WI_CORE_TRACE("Value of X:{0}, Y:{1}, Z{2}", parameter->x, parameter->y, parameter->z);
		*outParam = glm::normalize(*parameter);
	}
	void ScriptGlue::RegisterFunctions()
	{
		WI_ADD_INTERNAL_CALL(CppFunc);
		WI_ADD_INTERNAL_CALL(NativeLog);
		WI_ADD_INTERNAL_CALL(NativeLogVector);
	}
}