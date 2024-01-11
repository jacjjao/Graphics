#pragma once

#include "include/Core/Core.hpp"
#include "include/Events/ApplicationEvent.hpp"
#include "include/Core/Window.hpp"
#include "include/Core/Clock.hpp"
#include "src/Core/WorkerThread.hpp"
#include "LayerStack.hpp"

#include "include/Renderer/OrthographicCamera.hpp"
#include "include/Renderer/ShaderProgram.hpp"

namespace eg
{
	class Application
	{
	public:
		explicit Application(unsigned width, unsigned height, const std::string& title);

		~Application();

		static Application& getInstance() { return *s_instance; }

		Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

		void run();
		void onEvent(std::unique_ptr<eg::Event> e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void setTargetFps(unsigned fps);

		Window& getWindow() { return *m_window; }

		void setClearColor(const eg::Color color);

	private:
        void renderThreadLoop();
        void onDraw();
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;

		std::mutex m_layer_mutex;
		LayerStack m_layerStack;

		bool m_running = true;
        std::atomic_bool m_rendering = false;

		static Application* s_instance;

		std::atomic_bool m_fps_control = false;
        std::atomic<double> m_draw_interval = 0.0;
        eg::Clock m_draw_clock;

		WorkerThread m_render_thread;

		std::mutex         m_event_mutex;
		std::vector<std::unique_ptr<eg::Event>> m_event_buf;

		struct
		{
			float r, g, b, a;
        } m_clear_color{};
	};
} // namespace eg