#pragma once

#include "Core/Core.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Window.hpp"
#include "LayerStack.hpp"

namespace Engine
{
	class Application
	{
	public:
		virtual ~Application() = default;

		static Application& instance();

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

	protected:
		Application();

	private:
		bool onWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;

		LayerStack m_layerStack;

		bool m_running = true;
	};
} // namespace Engine