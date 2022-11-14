// third party
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <fmt/core.h>
// standard
#include <memory>
#include <cmath>
#include <numbers>

#include "../include/FileSystem.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Rectangle2D.hpp"
#include "../include/Utility.hpp"
#include "../include/Clock.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 1200;

std::unique_ptr<Rectangle2D> rect{};

int main()
{
    util::initialize(SCR_WIDTH, SCR_HEIGHT);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        fmt::print(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::print(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    ShaderProgram shaderProgram{FileSystem::getPath("/shader/shader.vert").c_str(),
                                FileSystem::getPath("/shader/shader.frag").c_str()};

    glm::ivec2 pos = {950, 550};
    rect = std::make_unique<Rectangle2D>(pos, 100, 100);
    rect->setColor(Color::White);

    Clock clock{};
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        auto tp = clock.getElapsedTime().asSeconds();
        Color color{};
        color.r = 255.0 * fabs(sin(tp));
        color.g = 255.0 * fabs(sin(tp + std::numbers::pi / 3.0));
        color.b = 255.0 * fabs(sin(tp + std::numbers::pi * 2.0 / 3.0));
        rect->setColor(color);
        rect->update();

        shaderProgram.use();
        rect->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.destroy();
    rect.reset();

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    auto pos = rect->getPosition();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos.y -= 1;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos.y += 1;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos.x -= 1;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos.x += 1;
        rect->setPosition(pos);
        rect->update();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
