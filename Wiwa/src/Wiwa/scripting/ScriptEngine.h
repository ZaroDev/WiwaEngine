#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <unordered_map>
#include "FileWatch.h"

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

		static void ReloadAssembly();

		static std::unordered_map<size_t, Type*>& getSystems();
		static std::unordered_map<size_t, Type*>& getComponents();
	
		struct ScriptEngineData
		{
			MonoDomain* RootDomain = nullptr;
			MonoDomain* AppDomain = nullptr;

			MonoAssembly* CoreAssembly = nullptr;
			MonoAssembly* SystemAssembly = nullptr;
			MonoAssembly* AppAssembly = nullptr;

			std::filesystem::path CoreAssemblyFilePath;
			std::filesystem::path AppAssemblyFilePath;

			std::unique_ptr<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
			bool AssemblyReloadPending = false;

			std::unordered_map<size_t, Type*> Components;
			std::unordered_map<size_t, Type*> Systems;
		};
		static ScriptEngineData* s_Data;

	private:
		static void InitMono();
		static void ShutDownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		static void LoadAssemblyTypes(MonoAssembly* assembly);

		static void ClearAssemblyTypes();

		friend class ScriptClass;
		friend class SystemClass;
	};


	
}