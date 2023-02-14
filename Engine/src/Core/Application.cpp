#include "include/Core/Application.hpp"
#include "include/Core/Log.hpp"
#include "include/Input/Input.hpp"
#include <glad/glad.h>

namespace eg
{
	Application* Application::s_instance = nullptr;

	void Application::run()
	{
		while (m_running)
		{
			glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
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
		m_window->setEventCallback([this](eg::Event& e) {
			onEvent(e);
		});
	}

	void Application::onEvent(eg::Event& e)
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

	bool Application::onWindowClosed(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	void Application::setClearColor(const Color color)
	{
		m_clear_color.r = float(color.r) / 255.0f;
		m_clear_color.g = float(color.g) / 255.0f;
		m_clear_color.b = float(color.b) / 255.0f;
		m_clear_color.a = float(color.a);
	}

} // namespace eg