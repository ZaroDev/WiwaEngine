#pragma once

#include "Wiwa/Window.h"

#include <GLFW/glfw3.h>

namespace Wiwa {
	class WindowsWindow : public Window
	{
	public: 
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		//Window attributes 
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width;
			uint32_t Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}