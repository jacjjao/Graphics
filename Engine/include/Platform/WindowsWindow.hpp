#pragma once

#include "include/Window.hpp"

struct GLFWwindow;

namespace Engine
{

	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;

		[[nodiscard]] unsigned GetWidth() const override { return m_Data.Width; }
		[[nodiscard]] unsigned GetHeight() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enabled) override;
		[[nodiscard]] bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned Width, Height;
			bool VSync;

			EventCallbackFn EventCallBack;
		};

		WindowData m_Data;
	};

}