#include <wipch.h>
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "ScriptClass.h"
#include "SystemClass.h"

#include <mono/metadata/object.h>
#include <glm/glm.hpp>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/systems/System.h>

#include <Wiwa/utilities/Reflection.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/class.h>
#include <Wiwa/core/Input.h>

#include <Wiwa/scene/SceneManager.h>

namespace Wiwa {

#define WI_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Wiwa.InternalCalls::" #Name, Name)

	void ClearName(std::string& str) {
		size_t ind = str.find('.');

		if (ind != str.npos) {
			str = str.substr(ind + 1, str.size() - ind - 1);
		}
	}

	Class* ConvertClass(MonoType* monotype, MonoClass* monoclass) {
		Class* t = new Class();
		t->is_class = true;
		t->is_array = false;
		t->is_enum = false;

		int alignment;

		// Fields
		t->field_count = mono_class_num_fields(monoclass);

		MonoClassField* monofield = NULL;
		MonoClass* monofieldclass = NULL;

		void* iter = NULL;
		size_t offset = 0;

		for (size_t i = 0; i < t->field_count; i++) {
			monofield = mono_class_get_fields(monoclass, &iter);
			monotype = mono_field_get_type(monofield);

			Field field;
			field.name = mono_field_get_name(monofield);
			field.offset = offset;
			field.type = ConvertType(monotype);

			t->fields.push_back(field);

			offset += mono_type_size(monotype, &alignment);
		}

		return t;
	}

	Type* ConvertType(MonoType* monotype) {
		MonoClass* monoclass = mono_type_get_class(monotype);

		Type* type = NULL;

		if (monoclass) {
			type = ConvertClass(monotype, monoclass);
		}
		else {
			type = new Type();
			type->is_enum = false;
			type->is_array = false;
			type->is_class = false;
		}

		int alignment = 0;

		std::string name = mono_type_get_name(monotype);
		ClearName(name);

		// Translate C# to C++
		if (name == "Single") {
			name = "float";
		}
		else if (name == "UInt32") {
			name = "unsigned int";
		}
		else if (name == "UInt64") {
			name = "unsigned __int64";
		}
		else if (name == "Int32") {
			name = "int";
		}
		else if (name == "Int64") {
			name = "__int64";
		}

		type->name = name;
		type->size = mono_type_size(monotype, &alignment);
		type->hash = std::hash<std::string>{}(type->name);

		return type;
	}

	void DestroyType(const Type* type);

	void DestroyClass(const Class* cl) {
		for (size_t i = 0; i < cl->field_count; i++) {
			DestroyType(cl->fields[i].type);
		}
	}

	void DestroyType(const Type* type) {
		if (type->is_class) {
			DestroyClass((Class*)type);
		}

		delete type;
	}

	void NativeLog(MonoString* string, int parameter)
	{
		char* str = mono_string_to_utf8(string);

		WI_CORE_TRACE("{0} {1}", str, parameter);
		mono_free(str);
	}
	void NativeLogVector(glm::vec3* parameter, glm::vec3* outParam)
	{
		WI_CORE_TRACE("Value of X:{0}, Y:{1}, Z{2}", parameter->x, parameter->y, parameter->z);
		*outParam = glm::normalize(*parameter);
	}

	/*MonoArray* GetComponent(EntityId id, MonoReflectionType* type)
	{
		static std::unordered_map<size_t, Type*> s_ConvertedTypes;

		MonoType* compType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(compType);
		ClearName(typeName);
		size_t typeHash = FNV1A_HASH(typeName.c_str());

		std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

		Type* t = NULL;

		if (converted_type == s_ConvertedTypes.end()) {
			t = ConvertType(compType);

			s_ConvertedTypes[typeHash] = t;
		}
		else {
			t = converted_type->second;
		}

		int alingment;

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		ComponentId compID = em.GetComponentId(t);
		byte* comp = em.GetComponent(id, compID, t->size);

		MonoArray* byteArray = NULL;

		if (comp) {
			SystemClass tmp("System", "Byte");

			byteArray = ScriptEngine::CreateArray(tmp.m_MonoClass, t->size);

			for (int i = 0; i < t->size; i++)
			{
				mono_array_set(byteArray, byte, i, comp[i]);
			}
		}

		return byteArray;
	}*/

	byte* GetComponent(EntityId id, MonoReflectionType* type) {
		static std::unordered_map<size_t, Type*> s_ConvertedTypes;

		MonoType* compType = mono_reflection_type_get_type(type);
		std::string typeName = mono_type_get_name(compType);
		ClearName(typeName);
		size_t typeHash = FNV1A_HASH(typeName.c_str());

		std::unordered_map<size_t, Type*>::iterator converted_type = s_ConvertedTypes.find(typeHash);

		Type* t = NULL;

		if (converted_type == s_ConvertedTypes.end()) {
			t = ConvertType(compType);

			s_ConvertedTypes[typeHash] = t;
		}
		else {
			t = converted_type->second;
		}

		int alingment;

		Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();

		ComponentId compID = em.GetComponentId(t);
		byte* comp = em.GetComponent(id, compID, t->size);

		return comp;
	}

	bool IsKeyDownIntr(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}
	bool IsMouseButtonPressedIntr(int button)
	{
		return Input::IsMouseButtonPressed(button);
	}

	void ScriptGlue::RegisterFunctions()
	{
		// Logging
		WI_ADD_INTERNAL_CALL(NativeLog);
		WI_ADD_INTERNAL_CALL(NativeLogVector);

		// Input
		WI_ADD_INTERNAL_CALL(IsKeyDownIntr);
		WI_ADD_INTERNAL_CALL(IsMouseButtonPressedIntr);

		// ECS
		WI_ADD_INTERNAL_CALL(GetComponent);
	}
}