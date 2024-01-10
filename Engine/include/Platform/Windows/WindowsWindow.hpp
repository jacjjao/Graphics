#pragma once

#include "include/Core/Window.hpp"

struct GLFWwindow;

namespace eg
{

	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void pollEvent() override;

		void swapBuffer() override;

		void setSize(unsigned width, unsigned height) override;

		[[nodiscard]] unsigned getWidth() const override { return m_data.width; }
		[[nodiscard]] unsigned getHeight() const override { return m_data.height; }

		void show() override;

		void setEventCallback(const EventCallbackFn& callback) override { m_data.eventCallBack = callback; }
		[[nodiscard]] void* getNativeWindowHandle() const override { return m_window; }

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_window;

		struct WindowData
		{
			std::string title;
			unsigned width, height;

			EventCallbackFn eventCallBack;
		};

		WindowData m_data{};
	};

}