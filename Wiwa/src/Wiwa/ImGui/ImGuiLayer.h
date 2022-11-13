#pragma once

#include <Wiwa/core/Layer.h>

#include "Wiwa/Events/Event.h"
#include "Wiwa/Events/KeyEvent.h"
#include "Wiwa/Events/MouseEvent.h"
#include "Wiwa/Events/ApplicationEvent.h"
#include "ImGuiLog.h"

struct ImGuiContext;

namespace Wiwa {
	class WI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Begin();
		void End();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;
		void OnImGuiRender()override;

		inline void LogTrace(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::TRACE, log); }
		inline void LogInfo(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::INFO, log); }
		inline void LogWarn(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::WARN, log); }
		inline void LogError(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::ERR, log); }
		inline void LogCritical(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::CRITICAL, log); }

		inline static void CoreLogTrace(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::TRACE, log); }
		inline static void CoreLogInfo(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::INFO, log); }
		inline static void CoreLogWarn(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::WARN, log); }
		inline static void CoreLogError(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::ERR, log); }
		inline static void CoreLogCritical(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::CRITICAL, log); }

		inline static ImGuiLog& GetLog() { return s_Log; }
		inline ImGuiLog& GetConsole() { return m_Console; }
		inline ImGuiContext* GetContext() { return m_Ctx; }
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMoveEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

		void SetDarkTheme();
	private:
		inline static ImGuiLog s_Log;
	private:
	    ImGuiLog m_Console;
		ImGuiContext* m_Ctx = nullptr;
		float m_Time = 0.0f;
	};
}