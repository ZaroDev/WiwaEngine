#pragma once

#include <Wiwa/core/Window.h>

#include <GLFW/glfw3.h>

namespace Wiwa {
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title = "";
			uint32_t Width = 0;
			uint32_t Height = 0;
			bool VSync = false;
			uint32_t Refresh = 0;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		WindowData m_OldData;

		bool m_Fullscreen = false;
		bool m_Resizable = false;
	public: 
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline WindowData& GetWindowData() { return m_Data; }

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline bool GetFullScreen() const override { return m_Fullscreen; }
		inline bool GetResizable() const override { return m_Resizable; }

		//Window attributes 
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;
		virtual void SetFullScreen(bool enabled) override;
		virtual void SetResizable(bool enabled) override;
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	};
}