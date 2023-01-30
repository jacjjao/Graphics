#include "include/Application.hpp"
#include "include/Log.hpp"
#include <glad/glad.h>

namespace Engine
{
	Application& Application::instance()
	{
		static Application app{};
		return app;
	}

	void Application::run()
	{
		while (m_running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_window->onUpdate();
		}
	}

	Application::Application()
	{
		m_window = Window::create();
		m_window->setEventCallback([this](Engine::Event& e) {
			onEvent(e);
		});
	}

	void Application::onEvent(Engine::Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {
			return onWindowClosed(e);
		});

		EG_CORE_TRACE("{}", e.toString());
	}

	bool Application::onWindowClosed(Engine::WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
} // namespace Engine