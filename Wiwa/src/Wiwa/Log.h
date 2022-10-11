#pragma once
#pragma warning(disable : 4251)
#include "Core.h"

#include "imgui.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Wiwa {
	class WI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static const char* GetCoreLastLog();
		static const char* GetClientLastLog();

		static void ImGuiLogTrace(const char* log);
		static void ImGuiLogInfo(const char* log);
		static void ImGuiLogWarn(const char* log);
		static void ImGuiLogError(const char* log);
		static void ImGuiLogCritical(const char* log);
	private:
		Log() = default;

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
#ifdef WI_DEBUG
//Core log macros
#define WI_CORE_TRACE(...)      ::Wiwa::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WI_CORE_INFO(...)       ::Wiwa::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WI_CORE_WARN(...)       ::Wiwa::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WI_CORE_ERROR(...)      ::Wiwa::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WI_CORE_CRITICAL(...)      ::Wiwa::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
#define WI_CORE_TRACE(...)   
#define WI_CORE_INFO(...)    
#define WI_CORE_WARN(...)    
#define WI_CORE_ERROR(...)   
#define WI_CORE_CRITICAL(...)
#endif


//Client log macros
#define WI_TRACE(...)           ::Wiwa::Log::GetClientLogger()->trace(__VA_ARGS__); ::Wiwa::Log::ImGuiLogTrace(::Wiwa::Log::GetClientLastLog())
#define WI_INFO(...)            ::Wiwa::Log::GetClientLogger()->info(__VA_ARGS__); ::Wiwa::Log::ImGuiLogInfo(::Wiwa::Log::GetClientLastLog())
#define WI_WARN(...)            ::Wiwa::Log::GetClientLogger()->warn(__VA_ARGS__); ::Wiwa::Log::ImGuiLogWarn(::Wiwa::Log::GetClientLastLog())
#define WI_ERROR(...)           ::Wiwa::Log::GetClientLogger()->error(__VA_ARGS__); ::Wiwa::Log::ImGuiLogError(::Wiwa::Log::GetClientLastLog())
#define WI_CRITICAL(...)           ::Wiwa::Log::GetClientLogger()->critical(__VA_ARGS__); ::Wiwa::Log::ImGuiLogCritical(::Wiwa::Log::GetClientLastLog())