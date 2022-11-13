#pragma once
#include <filesystem>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace Wiwa {
	namespace Utils {
		//TODO: Use the filesystem version Pablo you know what to do!
		char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize);
		MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath);
		MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		void PrintAssemblyTypes(MonoAssembly* assembly);
	}
}