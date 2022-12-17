#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "../include/Window.hpp"
#include "../include/TextRenderer.hpp"
#include "../include/Clock.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void character_callback(GLFWwindow* window, unsigned codepoint);

constexpr unsigned SCR_WIDTH  = 2400;
constexpr unsigned SCR_HEIGHT = 1200;

constexpr unsigned text_size = 40;

std::string str;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCharCallback(window, character_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    Window::initialize(SCR_WIDTH, SCR_HEIGHT);
    TextRenderer::initialize(text_size);

    str.reserve(1024);
    Clock clock{};
    constexpr double time_limit = 1.0 / 60.0; // 60fps
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwPollEvents();

        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        TextRenderer::renderText(str, 0.0F, 0.0F, Color::White, text_size);

        glfwSwapBuffers(window);

        while (clock.getElapsedTime().asSeconds() <= time_limit)
        {
            // do nothing
        }
        clock.restart();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void character_callback(GLFWwindow* window, unsigned codepoint)
{
    const char c = static_cast<char>(codepoint);
    str.push_back(c);
}

void framebuffer_size_callback(GLFWwindow* /*unused*/, int width, int height)
{
    glViewport(0, 0, width, height);
}