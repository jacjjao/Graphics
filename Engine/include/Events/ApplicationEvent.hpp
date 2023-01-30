#pragma once

#include "Event.hpp"

namespace Engine
{

	class WindowResizeEvent : public Event
	{
	public:
		explicit WindowResizeEvent(const unsigned width, const unsigned height) :
		m_Width(width),
		m_Height(height)
		{
		}

		[[nodiscard]] unsigned getWidth() const noexcept
		{
			return m_Width;
		}

		[[nodiscard]] unsigned getHeight() const noexcept
		{
			return m_Height;
		}

		[[nodiscard]] std::string toString() const override
		{
			std::stringstream ss{};
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}