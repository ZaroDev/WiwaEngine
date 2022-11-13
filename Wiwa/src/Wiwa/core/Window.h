#pragma once

#include "wipch.h"

#include <Wiwa/core/Core.h>
#include <Wiwa/events/Event.h>

namespace Wiwa {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		uint32_t Refresh;

		WindowProps(const std::string& title = "Wiwa Engine", 
			uint32_t width = 1280, 
			uint32_t height = 720,
			uint32_t refresh = 144) : Title(title), Width(width), Height(height), Refresh(refresh) {}
	};

	class WI_API Window
	{
	public:
		using EventCallbackFn = Action<Event&>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual bool GetFullScreen() const = 0;
		virtual bool GetResizable() const = 0;

		//Window atributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetFullScreen(bool enabled) = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void SetResizable(bool enabled) = 0;
		//Needs to be implemented per platform
		static Window* Create(const WindowProps& props = WindowProps());
	};
}