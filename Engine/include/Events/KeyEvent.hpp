#pragma once

#include "include/Core/Core.hpp"
#include "Event.hpp"

namespace eg
{
	
	class EG_API KeyEvent : public Event
	{
	public:
		[[nodiscard]] int GetKeyCode() const { return m_keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		explicit KeyEvent(const int keycode) :
			m_keycode(keycode)
		{
		}

		int m_keycode;
	};

	class EG_API KeyPressedEvent : public KeyEvent
	{
	public:
		explicit KeyPressedEvent(const int keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_isRepeat(isRepeat) {}

		[[nodiscard]] bool isRepeat() const { return m_isRepeat; }

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "KeyPressedEvent: " << m_keycode << " (repeat = " << m_isRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_isRepeat;
	};

	class EG_API KeyReleasedEvent : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {}

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "KeyReleasedEvent: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class EG_API KeyTypedEvent : public KeyEvent
	{
	public:
		explicit KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "KeyTypedEvent: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}