#include "include/Application.hpp"

#include <glad/glad.h>


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
		/*
		for (const auto& layer : m_LayerStack)
			layer->OnUpdate();

		m_Window->OnUpdate();*/
	}
}

Application::Application()
{
	// TODO: created window
}

void Application::onWindowClosed()
{
	m_running = false;
}