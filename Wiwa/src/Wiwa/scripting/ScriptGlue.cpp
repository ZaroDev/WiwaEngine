#include <wipch.h>
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include <mono/metadata/object.h>
#include <glm.hpp>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/Application.h>
#include <Wiwa/ecs/systems/System.h>

#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>

#include <Wiwa/Input.h>
namespace Wiwa {

#define WI_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Wiwa.InternalCalls::" #Name, Name)
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
	static MonoArray* GetComponent(EntityId id, MonoReflectionType* type)
	{
		MonoType* compType = mono_reflection_type_get_type(type);
		char* typeName = mono_type_get_name(compType);
		int alingment;
		int size = mono_type_size(compType, &alingment);
		//ComponentId compID = Application::Get().GetEntityManager().GetComponentId(compType);
		//byte* comp = Application::Get().GetEntityManager().GetComponent(id, 0, size);

		//MonoClass* compClass = mono_reflection_get;
		SystemClass tmp = SystemClass("System", "Byte");

		MonoArray* byteArray = ScriptEngine::CreateArray(tmp.m_MonoClass, size);
		Vector3f vec = { 40.f, 5.2f, 23.0f };
		byte* b = (byte*) & vec;

		for (int i = 0; i < size; i++)
		{
			mono_array_set(byteArray, byte, i, b[i]);
		}
		return byteArray;
	}
	static bool IsKeyDownIntr(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}
	static bool IsMouseButtonPressedIntr(int button)
	{
		return Input::IsMouseButtonPressed(button);
	}

	void ScriptGlue::RegisterFunctions()
	{
		WI_ADD_INTERNAL_CALL(NativeLog);
		WI_ADD_INTERNAL_CALL(NativeLogVector);
		WI_ADD_INTERNAL_CALL(GetComponent);
		WI_ADD_INTERNAL_CALL(IsKeyDownIntr);
		WI_ADD_INTERNAL_CALL(IsMouseButtonPressedIntr);
	}
}