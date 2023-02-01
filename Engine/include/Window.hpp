#pragma once

#include "Core/Core.hpp"
#include "Events/Event.hpp"

#include <functional>

namespace Engine
{
	struct WindowProps
	{
		std::string title = "Graphics Engine";
		unsigned width = 1280;
		unsigned height = 720;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		explicit Window() = default;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		[[nodiscard]] virtual unsigned getWidth() const = 0;
		[[nodiscard]] virtual unsigned getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		[[nodiscard]] virtual bool isVSync() const = 0;

		[[nodiscard]] virtual void* getNativeWindowHandle() const = 0;

		static std::unique_ptr<Window> create(const WindowProps& props = WindowProps{});
	};
} // namespace Engine