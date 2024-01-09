#pragma once

#include "include/Core/Core.hpp"
#include "include/Events/ApplicationEvent.hpp"
#include "include/Core/Window.hpp"
#include "include/Core/Clock.hpp"
#include "LayerStack.hpp"

#include "include/Renderer/OrthographicCamera.hpp"
#include "include/Renderer/ShaderProgram.hpp"

namespace eg
{
	class Application final
	{
	public:
		explicit Application(unsigned width, unsigned height, const std::string& title);

		static Application& getInstance() { return *s_instance; }

		Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void setTargetFps(unsigned fps);

		Window& getWindow() { return *m_window; }

		void setClearColor(const eg::Color color);

	private:
        void onDraw();
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;

		LayerStack m_layerStack;

		bool m_running = true;

		static Application* s_instance;

		bool      m_fps_control = false;
        double    m_draw_interval = 0.0;
        eg::Clock m_draw_clock;

		struct
		{
			float r, g, b, a;
		} m_clear_color;
	};
} // namespace eg