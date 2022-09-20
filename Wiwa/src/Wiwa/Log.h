#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Wiwa {
	class WI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger>s_CoreLogger;
		static std::shared_ptr<spdlog::logger>s_ClientLogger;
	};
}
//Core log macros
#define WI_CORE_TRACE(...)      ::Wiwa::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WI_CORE_INFO(...)       ::Wiwa::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WI_CORE_WARN(...)       ::Wiwa::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WI_CORE_ERROR(...)      ::Wiwa::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WI_CORE_FATAL(...)      ::Wiwa::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define WI_TRACE(...)           ::Wiwa::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WI_INFO(...)            ::Wiwa::Log::GetClientLogger()->info(__VA_ARGS__)
#define WI_WARN(...)            ::Wiwa::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WI_ERROR(...)           ::Wiwa::Log::GetClientLogger()->error(__VA_ARGS__)
#define WI_FATAL(...)           ::Wiwa::Log::GetClientLogger()->fatal(__VA_ARGS__)

