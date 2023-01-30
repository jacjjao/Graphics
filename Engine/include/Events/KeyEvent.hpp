#pragma once

#include "Event.hpp"

namespace Engine
{
	
	class KeyEvent : public Event
	{
	public:
		[[nodiscard]] int GetKeyCode() const { return m_int; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		explicit KeyEvent(const int keycode) :
		m_int(keycode)
		{
		}

		int m_int;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		explicit KeyPressedEvent(const int keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		[[nodiscard]] bool IsRepeat() const { return m_IsRepeat; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_int << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_int;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_int;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}