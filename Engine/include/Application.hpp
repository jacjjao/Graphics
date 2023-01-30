#pragma once

#include "Core/Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Window.hpp"

namespace Engine
{

	class Application
	{
	public:
		virtual ~Application() = default;

		static Application& instance();

		void run();
		void onEvent(Event& e);

	private:
		explicit Application();
		bool onWindowClosed(WindowCloseEvent& e);

		static Application* s_instanced;

		std::unique_ptr<Window> m_window;

		bool m_running = true;
	};

} // namespace Engine