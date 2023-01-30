#pragma once

#include <string>

#define BIT(x) (1 << x)

namespace Engine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		EventCategoryNone		 = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput	     = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; } \
								   EventType getEventType() const override { return EventType::type; } \
								   const char* getName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) int getCategoryFlag() const override { return EventCategory::category; }

	class Event
	{
	private:
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		[[nodiscard]] virtual const char* getName() const = 0;
		[[nodiscard]] virtual EventType getEventType() const = 0;
		[[nodiscard]] virtual int getCategoryFlag() const = 0;

		[[nodiscard]] virtual std::string toString() const { return getName(); }

		[[nodiscard]] bool isInCategory(EventCategory category) const
		{
			return getCategoryFlag() & category;
		}

		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		explicit EventDispatcher(Event& event) : m_event(event)
		{
		}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

}