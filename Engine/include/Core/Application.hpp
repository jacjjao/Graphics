#pragma once

#include "include/Core/Core.hpp"
#include "include/Events/ApplicationEvent.hpp"
#include "include/Core/Window.hpp"
#include "LayerStack.hpp"

#include "include/Renderer/OrthographicCamera.hpp"
#include "include/Renderer/ShaderProgram.hpp"
#include "include/Renderer/Rectangle2D.hpp"

namespace eg
{
	class Application
	{
	public:
		explicit Application();
		virtual ~Application() = default;

		static Application& getInstance() { return *s_instance; }

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		Window& getWindow() { return *m_window; }

		void setClearColor(const eg::Color color);

	private:
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;

		LayerStack m_layerStack;

		bool m_running = true;

		static Application* s_instance;

		struct
		{
			float r, g, b, a;
		} m_clear_color;
	};
} // namespace eg