#include <wipch.h>
#include "ScriptEngine.h"

#include <glm.hpp>
#include "ScriptGlue.h"

#include "ScriptUtils.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>
#include <mono/metadata/reflection.h>
#include <optick.h>
#include <Wiwa/utilities/Reflection.h>

namespace Wiwa {
	ScriptEngine::ScriptEngineData* ScriptEngine::s_Data = nullptr;
	
	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		LoadAssembly("resources/scripts/Wiwa-ScriptCore.dll");
		LoadAppAssembly("resources/scripts/Wiwa-AppAssembly.dll");
		WI_CORE_WARN("Components");
		Utils::PrintReflectionTypes(s_Data->Components);
		WI_CORE_WARN("Systems");
		Utils::PrintReflectionTypes(s_Data->Systems);
		ScriptGlue::RegisterFunctions();
	}
	void ScriptEngine::ShutDown()
	{
		ShutDownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("WiwaScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string().c_str());
		s_Data->SystemAssembly = Utils::LoadMonoAssembly("mono/lib/mono/4.5/mscorlib.dll");
		LoadAssemblyTypes(s_Data->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_Data->SystemAssembly);
		
		//WI_CORE_ASSERT(false, "");
	}

	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath.string().c_str());
		LoadAssemblyTypes(s_Data->AppAssembly);
	}
	
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("WiwaJITRuntime");
		
		WI_CORE_ASSERT(rootDomain, "Mono root domain not initialized!");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;
	}
	void ScriptEngine::ShutDownMono()
	{
		//Mono crashes when shutting down bc mono = monkey code

		/*mono_domain_unload(s_Data->AppDomain);*/
		s_Data->AppDomain = nullptr;

		/*mono_jit_cleanup(s_Data->RootDomain);*/
		s_Data->RootDomain = nullptr;
	}

	MonoArray* ScriptEngine::CreateArray(MonoClass* type, uint32_t size)
	{
		return mono_array_new(s_Data->AppDomain, type, (uintptr_t)size);
	}
	std::unordered_map<size_t, Type*> ScriptEngine::getSystems()
	{
		return s_Data->Systems;
	}
	std::unordered_map<size_t, Type*> ScriptEngine::getComponents()
	{
		return s_Data->Components;
	}
	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}
	void ScriptEngine::LoadAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* systemClass = Utils::GetClassInAssembly(s_Data->CoreAssembly, "Wiwa", "Behaviour");
		MonoClass* componentClass = Utils::GetClassInAssembly(s_Data->CoreAssembly, "Wiwa", "Component");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = Utils::GetClassInAssembly(assembly, nameSpace, name);

			if (monoClass == systemClass || monoClass == componentClass)
				continue;
			
			mono_bool isEnum = mono_class_is_enum(monoClass);
			if (isEnum == 1)
				continue;

			bool isSystem = mono_class_is_subclass_of(monoClass, systemClass, false);

			MonoType* monoType = mono_class_get_type(monoClass);

			Type* type = ConvertType(monoType);

			if (isSystem)
			{
				s_Data->Systems[type->hash] = type;
				continue;
			}
			MonoCustomAttrInfo* attributes = mono_custom_attrs_from_class(monoClass);
			if (attributes == nullptr)
				continue;

			mono_bool isComponent = mono_custom_attrs_has_attr(attributes, componentClass);
			if (isComponent == 1)
			{
				s_Data->Components[type->hash] = type;
			}
		}
	}
}