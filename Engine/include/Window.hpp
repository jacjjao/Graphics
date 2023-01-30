#pragma once

#include "Core/Core.hpp"
#include "Events/Event.hpp"

#include <functional>

namespace Engine
{
	
	struct WindowProps
	{
		std::string Title;
		unsigned Width;
		unsigned Height;

		explicit WindowProps(std::string_view title = "Graphics Engine",
							 unsigned width = 1280,
							 unsigned height = 720)
		: Title{ title }, Width{ width }, Height{ height }
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		explicit Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		[[nodiscard]] virtual unsigned GetWidth() const = 0;
		[[nodiscard]] virtual unsigned GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps{});
	};

}