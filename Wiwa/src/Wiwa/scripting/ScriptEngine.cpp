#include <wipch.h>
#include "ScriptEngine.h"

#include <glm.hpp>
#include "ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <Wiwa/FileSystem.h>

namespace Wiwa {
	namespace Utils {
		//TODO: Use the filesystem version Pablo you know what to do!
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				WI_CORE_ERROR("ScriptEngine {0}", errorMessage);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}
		static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			MonoClass* klass = mono_class_from_name(image, namespaceName, className);

			if (klass == nullptr)
			{
				WI_CORE_ERROR("Mono can't get the class {0}.{1} from the assembly image ", namespaceName, className);
				return nullptr;
			}

			return klass;
		}
	}

	
	struct ScriptEngineData
	{
		MonoDomain* RootDomain;
		MonoDomain* AppDomain;

		MonoAssembly* CoreAssembly = nullptr;
		MonoAssembly* SystemAssembly = nullptr;

		MonoDomain* SystemDomain = nullptr;

		ScriptClass EnityClass;
	};
	
	static ScriptEngineData* s_Data = nullptr;
	
	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		File file = FileSystem::OpenO("assembly.txt");
		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			file << nameSpace;
			file << name;
			file << "\n";

			WI_CORE_INFO("{0}.{1}", nameSpace, name);
		}
		file.Close();
	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		LoadAssembly("resources/scripts/Wiwa-ScriptCore.dll");
		ScriptGlue::RegisterFunctions();
		s_Data->EnityClass = ScriptClass("Wiwa", "Behaviour");
		
		MonoObject* instance = s_Data->EnityClass.Instantiate();

		MonoMethod* printMessageFunc = s_Data->EnityClass.GetMethod("OnUpdate", 2);
		int value = 2;
		void* params[2]
		{
			&value,
			&value
		};
		s_Data->EnityClass.InvokeMethod(instance, printMessageFunc, params);
		
	/*	MonoMethod* printIntFunc = s_Data->EnityClass.GetMethod("PrintInt", 1);
		int value = 5;
		void* param = &value;
		s_Data->EnityClass.InvokeMethod(instance,printIntFunc, &param);*/
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
		//PrintAssemblyTypes(s_Data->CoreAssembly);
		//PrintAssemblyTypes(s_Data->SystemAssembly);
		//WI_CORE_ASSERT(false, "");
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
	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = Utils::GetClassInAssembly(s_Data->CoreAssembly, classNamespace.c_str(), className.c_str());
	}
	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount) 
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}
	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}


	SystemClass::SystemClass(const std::string& classNamespace, const std::string& className)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = Utils::GetClassInAssembly(s_Data->SystemAssembly, classNamespace.c_str(), className.c_str());
	}
	MonoObject* SystemClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}
	MonoMethod* SystemClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}
	MonoObject* SystemClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

}