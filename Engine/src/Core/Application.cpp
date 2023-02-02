#include "include/Core/Application.hpp"
#include "include/Core/Log.hpp"
#include "include/Input/Input.hpp"
#include <glad/glad.h>

namespace Engine
{
	Application* Application::s_instance = nullptr;

	void Application::run()
	{
		while (m_running)
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (const auto& layer : m_layerStack)
				layer->onUpdate();

			m_window->onUpdate();
		}
	}

	Application::Application()
	{
		EG_CORE_ASSERT(s_instance == nullptr, "Application is already exist!");
		s_instance = this;

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
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {
			glViewport(0, 0, (GLsizei)e.getWidth(), (GLsizei)e.getHeight());
			return true;	
		});

		for (const auto& layer : m_layerStack | std::views::reverse)
		{
			layer->onEvent(e);
			if (e.handled)
				break;
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_layerStack.pushOverlay(overlay);
	}

	bool Application::onWindowClosed(Engine::WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
} // namespace Engine