#include <wipch.h>
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Wiwa {
	struct ScriptEngineData
	{
		MonoDomain* RootDomain;
		MonoDomain* AppDomain;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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
	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}
	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			WI_CORE_INFO("{0}.{1}", nameSpace, name);
		}
	}
	MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		if (klass == nullptr)
		{
			WI_CORE_ERROR("Mono can't get the class {0} from the assembly image {1}", className);
			return nullptr;
		}

		return klass;
	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
	}
	void ScriptEngine::ShutDown()
	{
		ShutDownMono();
		delete s_Data;
	}
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("WiwaJITRuntime");
		
		WI_CORE_ASSERT(rootDomain, "Mono root domain not initialized!");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("WiwaScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadCSharpAssembly("resources/scripts/Wiwa-ScriptCore.dll");

		PrintAssemblyTypes(s_Data->CoreAssembly);

		//Get the class
		MonoClass* coreClass = GetClassInAssembly(s_Data->CoreAssembly, "Wiwa", "Main");
		//Create an object
		MonoObject* instance = mono_object_new(s_Data->AppDomain, coreClass);
		mono_runtime_object_init(instance);
		
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(coreClass, "PrintLine", 0);
		mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

		MonoMethod* printIntFunc = mono_class_get_method_from_name(coreClass, "PrintInt", 1);
		int value = 5;
		void* param = &value;
		mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

		MonoString* string = mono_string_new(s_Data->AppDomain, "Hello world from C++");
		void* stringParam = string;
		MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(coreClass, "PrintCustomLine", 1);
		mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
	}
	void ScriptEngine::ShutDownMono()
	{
		//Mono crashes when shutting down bc mono = monkey code
		/*mono_domain_unload(s_Data->AppDomain);*/
		s_Data->AppDomain = nullptr;

		/*mono_jit_cleanup(s_Data->RootDomain);*/
		s_Data->RootDomain = nullptr;
	}
}