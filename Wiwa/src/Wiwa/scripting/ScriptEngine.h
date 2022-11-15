#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

struct Type;

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoArray MonoArray;
	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoAssembly MonoAssembly;
}


namespace Wiwa {
	class WI_API ScriptEngine
	{
	public:
		static void Init();
		static void ShutDown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static MonoArray* CreateArray(MonoClass* type, uint32_t size);

		static std::unordered_map<size_t, Type*> getSystems();
		static std::unordered_map<size_t, Type*> getComponents();
	private:
		struct ScriptEngineData
		{
			MonoDomain* RootDomain;
			MonoDomain* AppDomain;

			MonoAssembly* CoreAssembly = nullptr;
			MonoAssembly* SystemAssembly = nullptr;
			MonoAssembly* AppAssembly = nullptr;

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