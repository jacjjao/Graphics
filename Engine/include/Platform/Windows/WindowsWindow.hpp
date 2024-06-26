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

		void onUpdate() override;

		[[nodiscard]] unsigned getWidth() const override { return m_data.width; }
		[[nodiscard]] unsigned getHeight() const override { return m_data.height; }

		void setEventCallback(const EventCallbackFn& callback) override { m_data.eventCallBack = callback; }
		void setVSync(bool enabled) override;
		[[nodiscard]] bool isVSync() const override;
		[[nodiscard]] void* getNativeWindowHandle() const override { return m_window; }

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_window;

		struct WindowData
		{
			std::string title;
			unsigned width, height;
			bool vSync;

			EventCallbackFn eventCallBack;
		};

		WindowData m_data;
	};

}