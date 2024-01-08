#include "pch.hpp"
#include "include/Core/Application.hpp"
#include "include/Core/Log.hpp"
#include "include/Input/Input.hpp"
#include "include/Core/Math.hpp"

namespace eg
{
	Application* Application::s_instance = nullptr;

	void Application::run()
	{
		while (m_running)
		{
			const auto [r, g, b, a] = m_clear_color;
			glClearColor(r, g, b, a);
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

		const float hw = static_cast<float>(m_window->getWidth()) / 2.0f;
		const float hh = static_cast<float>(m_window->getHeight()) / 2.0f;
		auto& shader = DefaultShaderProgram::instance();
		shader.use();
		shader.setMat4("view", Constants::identity_mat4);
		shader.setMat4("proj", ortho(-hw, hw, -hh, hh, -1.0f, 1.0f));
		shader.unuse();
	}

	void Application::onEvent(eg::Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {
			return onWindowClosed(e);
		});
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {
			glViewport(0, 0, static_cast<GLsizei>(e.getWidth()), static_cast<GLsizei>(e.getHeight()));
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

	bool Application::onWindowClosed(WindowCloseEvent&)
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