#pragma once

#include "include/Core/Core.hpp"
#include "include/Events/Event.hpp"

#include <functional>

namespace eg
{
	struct WindowProps
	{
		std::string title = "Graphics eg";
		unsigned width = 2000;
		unsigned height = 1200;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		explicit Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		virtual ~Window() = default;

		virtual void pollEvent() = 0;

		virtual void swapBuffer() = 0;

		virtual void setSize(unsigned width, unsigned height) = 0;

		virtual void show() = 0;

		[[nodiscard]] 
		virtual unsigned getWidth() const = 0;
		[[nodiscard]]  
		virtual unsigned getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		[[nodiscard]] 
		virtual bool isVSync() const = 0;

		[[nodiscard]] 
		virtual void* getNativeWindowHandle() const = 0;

		static std::unique_ptr<Window> create(const WindowProps& props = WindowProps{});
	};
} // namespace eg