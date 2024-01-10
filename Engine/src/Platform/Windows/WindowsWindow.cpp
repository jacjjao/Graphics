#include "pch.hpp"
#include "include/Platform/Windows/WindowsWindow.hpp"
#include "include/Core/Log.hpp"

#include "include/Events/ApplicationEvent.hpp"
#include "include/Events/KeyEvent.hpp"
#include "include/Events/MouseEvent.hpp"

namespace eg
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EG_CORE_ERROR("GLFW Error: ({}): {}", error, description);
	}

	static void APIENTRY GLDebugMessageCallback(
        GLenum                       source,
        GLenum                       type,
        GLuint                       id,
        GLenum                       severity,
        [[maybe_unused]] GLsizei     length,
        const GLchar*                msg,
        [[maybe_unused]] const void* data)
	{
		static std::string _source;
		static std::string _type;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			EG_CORE_ERROR("{}: {} of {} severity, raised from {}: {}", id, _type, "HIGH", _source, msg);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			EG_CORE_WARN("{}: {} of {} severity, raised from {}: {}", id, _type, "MEDIUM", _source, msg);
			break;

		case GL_DEBUG_SEVERITY_LOW:
			EG_CORE_TRACE("{}: {} of {} severity, raised from {}: {}", id, _type, "LOW", _source, msg);
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			EG_CORE_INFO("{}: {} of {} severity, raised from {}: {}", id, _type, "INFO", _source, msg);
			break;

		default:
			EG_CORE_ERROR("{}: {} of {} severity, raised from {}: {}", id, _type, "FATAL", _source, msg);
			break;
		}
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
		// copy data
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;
		EG_CORE_INFO("Creating window {} ({}, {})", props.title, props.width, props.height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			EG_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);

		// center the window
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_window, mode->width / 2 - m_data.width / 2, mode->height / 2 - m_data.height / 2);

		glfwSetWindowUserPointer(m_window, &m_data);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, const int width, const int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;
			data.eventCallBack(std::make_unique<WindowResizeEvent>(width, height));
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.eventCallBack(std::make_unique<WindowCloseEvent>());
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, [[maybe_unused]]const int scancode, const int action, [[maybe_unused]]const int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch(action)
			{
			case GLFW_PRESS:
			{
                data.eventCallBack(std::make_unique<KeyPressedEvent>(key, false));
				break;
			}
			case GLFW_RELEASE:
			{
                data.eventCallBack(std::make_unique<KeyReleasedEvent>(key));
				break;
			}
			case GLFW_REPEAT:
			{
                data.eventCallBack(std::make_unique<KeyPressedEvent>(key, true));
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, const int button, const int action, [[maybe_unused]] const int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
                data.eventCallBack(std::make_unique<MouseButtonPressedEvent>(button));
				break;
			}
			case GLFW_RELEASE:
			{
                data.eventCallBack(std::make_unique<MouseButtonReleasedEvent>(button));
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.eventCallBack(std::make_unique<MouseScrolledEvent>(xOffset, yOffset));
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double xPos, const double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.eventCallBack(std::make_unique<MouseMovedEvent>(xPos, yPos));
		});
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::pollEvent()
	{
		glfwPollEvents();
    }

	void WindowsWindow::swapBuffer()
	{
        glfwSwapBuffers(m_window);
	}

    void WindowsWindow::setSize(unsigned width, unsigned height)
    {
        m_data.width = width;
        m_data.height = height;
        glfwSetWindowSize(m_window, width, height);
    }

	void WindowsWindow::show()
    {
        glfwShowWindow(m_window);
    }

    void WindowsWindow::makeContextCurrent()
    {
        glfwMakeContextCurrent(m_window);

		static bool glad_init = false;
        if (glad_init)
            return;
        auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EG_CORE_ASSERT(status, "Failed to initialized Glad!");
#ifdef EG_ENABLE_DEBUG_OUTPUT
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugMessageCallback, nullptr);
#endif
        glad_init = true;
    }

} // namespace eg
