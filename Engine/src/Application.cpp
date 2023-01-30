#include "include/Application.hpp"
#include "include/Log.hpp"
#include "include/Platform/WindowsWindow.hpp"
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

			m_window->OnUpdate();
		}
	}

	Application::Application()
	{
		m_window = std::make_unique<Engine::WindowsWindow>(Engine::WindowProps{});
		m_window->SetEventCallback([this](Engine::Event& e) {
			onEvent(e);
		});
	}

	void Application::onEvent(Engine::Event& e)
	{
		Engine::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Engine::WindowCloseEvent>([this](Engine::WindowCloseEvent& e) {
			return onWindowClosed(e);
		});

		//EG_CORE_TRACE("{}", e.ToString());
	}

	bool Application::onWindowClosed(Engine::WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
} // namespace Engine