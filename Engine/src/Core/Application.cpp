#include "pch.hpp"
#include "include/Core/Application.hpp"
#include "include/Core/Log.hpp"
#include "include/Input/Input.hpp"
#include "include/Core/Math.hpp"
#include "include/Renderer/Renderer2D.hpp"
#include "include/Renderer/TextRenderer.hpp"

namespace eg
{
	Application* Application::s_instance = nullptr;

	Application::Application(const unsigned width, const unsigned height, const std::string& title)
    {
        Log::init();

        EG_CORE_ASSERT(s_instance == nullptr, "Application is already exist!");
        s_instance = this;

        eg::WindowProps props{.title = title, .width = width, .height = height};
        m_window = Window::create(props);
        m_window->setEventCallback([this](std::unique_ptr<eg::Event> e) { onEvent(std::move(e)); });

		m_render_thread.start();
		m_render_thread.assignJob([this] 
        { 
            m_window->makeContextCurrent(); 
            Renderer2D::init();
            TextRenderer::init(20);
        });
    }

    Application::~Application()
    {
        m_running = false;
        m_render_thread.join();
    }

	void Application::onDraw()
    {
        const auto [r, g, b, a] = m_clear_color;
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        for (const auto& layer : m_layerStack)
            layer->onDraw();
        m_window->swapBuffer();
    }

    void Application::renderThreadLoop()
    {
        while (m_rendering)
        {
            std::lock_guard layer_lock(m_layer_mutex);
            for (const auto& layer : m_layerStack)
                layer->onUpdate();

            {
                std::lock_guard event_lock(m_event_mutex);
                for (const auto& e : m_event_buf)
                {
                    if (e->getEventType() == EventType::WindowResize)
                    {
                        const auto event = static_cast<const WindowResizeEvent*>(e.get());
                        glViewport(0, 0, static_cast<GLsizei>(event->getWidth()), static_cast<GLsizei>(event->getHeight()));
                        continue;
                    }
                    for (const auto& layer : m_layerStack)
                    {
                        if (e->handled)
                            break;
                        layer->onEvent(*e);
                    }
                }
                m_event_buf.clear();
            }

            if (!m_fps_control || m_draw_clock.getElapsedTime().asSeconds() >= m_draw_interval)
            {
                m_draw_clock.restart();
                onDraw();
            }
        }
    }

	void Application::run()
    {
        m_rendering = true;
        m_render_thread.assignJob([this] { renderThreadLoop(); });
        m_window->show();
		while (m_running)
		{
			m_window->pollEvent();
		}
	}

	void Application::onEvent(std::unique_ptr<eg::Event> e)
	{
		EventDispatcher dispatcher(*e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {
			return onWindowClosed(e);
		});

        if (e->handled)
            return;
        {
            std::lock_guard lock(m_event_mutex);
            m_event_buf.emplace_back(std::move(e));
        }
	}

	void Application::pushLayer(Layer* layer)
	{
        std::lock_guard lock(m_layer_mutex);
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
        std::lock_guard lock(m_layer_mutex);
		m_layerStack.pushOverlay(overlay);
	}

	void Application::setTargetFps(const unsigned fps)
	{
		if (fps == 0)
		{
            m_fps_control = false;
            return;
		}
        m_fps_control = true;
        m_draw_interval = 1.0 / static_cast<double>(fps);
	}

	bool Application::onWindowClosed(WindowCloseEvent&)
	{
		m_running = false;
        m_rendering = false;
        m_render_thread.stop();
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