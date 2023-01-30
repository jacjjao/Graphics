#pragma once

#include "Event.hpp"

namespace Engine
{
	class MouseMovedEvent : public Event
	{
	public:
		explicit MouseMovedEvent(const float x, const float y)
			: m_mouseX(x), m_mouseY(y) {}

		float getX() const { return m_mouseX; }
		float getY() const { return m_mouseY; }

		std::string toString() const override
		{
			std::stringstream ss{};
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_mouseX, m_mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		explicit MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		[[nodiscard]] float getXOffset() const { return m_xOffset; }
		[[nodiscard]] float getYOffset() const { return m_yOffset; }

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_xOffset, m_yOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		[[nodiscard]] int getMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		explicit MouseButtonEvent(const int button)
			: m_button(button) {}

		int m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) {}

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) {}

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}