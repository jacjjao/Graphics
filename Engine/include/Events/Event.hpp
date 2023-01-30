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

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								   EventType GetEventType() const override { return EventType::type; } \
								   const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) int GetCategoryFlag() const override { return EventCategory::category; }

	class Event
	{
	private:
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;

		[[nodiscard]] virtual const char* GetName() const = 0;
		[[nodiscard]] virtual EventType GetEventType() const = 0;
		[[nodiscard]] virtual int GetCategoryFlag() const = 0;

		[[nodiscard]] virtual std::string ToString() const { return GetName(); }

		[[nodiscard]] bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlag() & category;
		}

		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		explicit EventDispatcher(Event& event) : m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}