#include "include/Platform/Windows/WindowsInput.hpp"

#include "include/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace eg
{
	
	Input* Input::s_instance = new WindowsInput{};

	bool WindowsInput::isKeyPressedImpl(const int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindowHandle());
		const auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(const int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindowHandle());
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindowHandle());
		double xPos = 0, yPos = 0;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

} // eg