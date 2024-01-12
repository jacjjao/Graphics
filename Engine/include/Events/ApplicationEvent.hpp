#pragma once

#include "include/Core/Core.hpp"
#include "Event.hpp"
#include <sstream>

namespace eg
{

	class EG_API WindowResizeEvent : public Event
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

	class EG_API WindowCloseEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class EG_API AppTickEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class EG_API AppUpdateEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class EG_API AppRenderEvent : public Event
	{
	public:
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}