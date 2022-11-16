#include "wipch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <Wiwa/imgui/ImGuiLayer.h>
#include <Wiwa/imgui/ImGuiLog.h>
#include <Wiwa/core/Application.h>

namespace Wiwa {
	std::shared_ptr<spdlog::logger>Log::s_CoreLogger = nullptr;
	std::shared_ptr<spdlog::logger>Log::s_ClientLogger = nullptr;

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
		ImGuiLayer::CoreLogTrace(log);
	}

	void Log::ImGuiLogInfo(const char* log)
	{
		ImGuiLayer::CoreLogInfo(log);
	}
	void Log::ImGuiLogWarn(const char* log)
	{
		ImGuiLayer::CoreLogWarn(log);
	}
	void Log::ImGuiLogError(const char* log)
	{
		ImGuiLayer::CoreLogError(log);
	}
	void Log::ImGuiLogCritical(const char* log)
	{
		ImGuiLayer::CoreLogCritical(log);
	}
	void Log::ImGuiConsoleTrace(const char* log)
	{
		Application::Get().GetImGuiLayer().LogTrace(log);
	}
	void Log::ImGuiConsoleInfo(const char* log)
	{
		Application::Get().GetImGuiLayer().LogInfo(log);
	}
	void Log::ImGuiConsoleWarn(const char* log)
	{
		Application::Get().GetImGuiLayer().LogWarn(log);
	}
	void Log::ImGuiConsoleError(const char* log)
	{
		Application::Get().GetImGuiLayer().LogError(log);
	}
	void Log::ImGuiConsoleCritical(const char* log)
	{
		Application::Get().GetImGuiLayer().LogCritical(log);
	}
}