#include "pch.hpp"
#include "WindowsWindow.hpp"
#include "include/Log.hpp"

#include "include/Events/ApplicationEvent.hpp"
#include "include/Events/KeyEvent.hpp"
#include "include/Events/MouseEvent.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Engine
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EG_CORE_ERROR("GLFW Error: ({}): {}", error, description);
	}

	std::unique_ptr<Window> Window::create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		EG_CORE_INFO("Creating window {} ({}, {})", props.title, props.width, props.height);

		if (not s_GLFWInitialized)
		{
			int success = glfwInit();
			EG_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EG_CORE_ASSERT(status, "Failed to initialized Glad!");
		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, const int width, const int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallBack(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event{};
			data.eventCallBack(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch(action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				data.eventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallBack(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.eventCallBack(event);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, const int button, const int action, const int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallBack(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallBack(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(xOffset, yOffset);
			data.eventCallBack(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double xPos, const double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(xPos, yPos);
			data.eventCallBack(event);
		});
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::setVSync(const bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_data.vSync;
	}
} // namespace Engine
