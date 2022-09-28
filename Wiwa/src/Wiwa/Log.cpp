#include "wipch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Application.h"

namespace Wiwa {
	std::shared_ptr<spdlog::logger>Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger>Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		s_CoreLogger = spdlog::stdout_color_mt("WIWA");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);		
	}
	const char* Log::GetCoreLastLog()
	{
		const std::vector<spdlog::sink_ptr>& sinks = s_CoreLogger.get()->sinks();

		return sinks[0].get()->get_last();
	}
	const char* Log::GetClientLastLog()
	{
		const std::vector<spdlog::sink_ptr>& sinks = s_ClientLogger.get()->sinks();

		return sinks[0].get()->get_last();
	}

	void Log::ImGuiLogTrace(const char* log)
	{
		Application::Get().GetImGuiLayer()->LogTrace(log);
	}

	void Log::ImGuiLogInfo(const char* log)
	{
		Application::Get().GetImGuiLayer()->LogInfo(log);
	}
	void Log::ImGuiLogWarn(const char* log)
	{
		Application::Get().GetImGuiLayer()->LogWarn(log);
	}
	void Log::ImGuiLogError(const char* log)
	{
		Application::Get().GetImGuiLayer()->LogError(log);
	}
	void Log::ImGuiLogCritical(const char* log)
	{
		Application::Get().GetImGuiLayer()->LogCritical(log);
	}
}