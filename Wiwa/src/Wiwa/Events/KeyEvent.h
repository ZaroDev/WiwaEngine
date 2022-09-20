#pragma once

#include "Event.h"
#include "Wiwa/KeyCodes.h"

namespace Wiwa {
	class WI_API KeyEvent : public Event
	{
	public: 
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class WI_API KeyPressedEvent : public KeyEvent
	{
	public: 
		KeyPressedEvent(KeyCode keycode, KeyCode repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline KeyCode GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		KeyCode m_RepeatCount;
	};

	class WI_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}