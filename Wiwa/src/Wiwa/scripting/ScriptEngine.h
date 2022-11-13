#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <Wiwa/utilities/Reflection.h>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoAssembly MonoAssembly;
}


namespace Wiwa {
	

	class ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static MonoArray* CreateArray(MonoClass* type, uint32_t size);
	private:
		struct ScriptEngineData
		{
			MonoDomain* RootDomain;
			MonoDomain* AppDomain;

			MonoAssembly* CoreAssembly = nullptr;
			MonoAssembly* SystemAssembly = nullptr;
			MonoAssembly* UserAssembly = nullptr;

			std::unordered_map<size_t, Type*> Components;
			std::unordered_map<size_t, Type*> Systems;
		};
		static ScriptEngineData* s_Data;

		static void InitMono();
		static void ShutDownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static void LoadAssemblyTypes(MonoAssembly* assembly);

		friend class ScriptClass;
		friend class SystemClass;
	};


	
}