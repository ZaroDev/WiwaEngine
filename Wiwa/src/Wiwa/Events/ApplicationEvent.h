#pragma once

#include "Event.h"

namespace Wiwa
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class WindowDropEvent : public Event
	{
	public:
		WindowDropEvent(int count, const char** paths)
			: m_Count(count), m_Paths(paths){}
		int GetCount() const { return m_Count; }
		const char** GetPaths() const { return m_Paths; }

		EVENT_CLASS_TYPE(WindowDrop)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_Count;
		const char** m_Paths;
	};
	class SceneChangeEvent : public Event 
	{
	public:
		SceneChangeEvent(size_t id) 
			: m_Id(id){}

		size_t GetID() const { return m_Id; }

		EVENT_CLASS_TYPE(SceneChange)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		size_t m_Id;
	};
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	

	class OnLoadEvent : public Event
	{
	public:
		OnLoadEvent() = default;

		EVENT_CLASS_TYPE(OnLoad)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class OnSaveEvent : public Event
	{
	public:
		OnSaveEvent() = default;

		EVENT_CLASS_TYPE(OnSave)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
