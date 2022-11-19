// third party
#include <cstdio>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_uint2.hpp>
// standard
#include <iostream>
#include <memory>
#include <cmath>
#include <numbers>

#include "../include/ShaderProgram.hpp"
#include "../include/Rectangle2D.hpp"
#include "../include/Utility.hpp"
#include "../include/Clock.hpp"
#include "../include/Circle2D.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 1200;

std::unique_ptr<Rectangle2D> rect{};

int main()
{
    Utility::initialize(SCR_WIDTH, SCR_HEIGHT);

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    auto& shaderProgram = ShaderProgram2D::instance();

    glm::vec2 pos = {300, 1000};
    rect = std::make_unique<Rectangle2D>(100, 100);
    rect->setPosition(pos);

    pos = {1800, 900};
    auto circle = std::make_unique<Circle2D>(50.0F);
    circle->setPosition(pos);

    VertexArray vao{3};
    vao[0].position = {1000.0, 100.0};
    vao[1].position = {1000.0 + 500.0 * std::sqrt(3.0) / 2.0, 850.0};
    vao[2].position = {1000.0 - 500.0 * std::sqrt(3.0) / 2.0, 850.0};

    circle->scale({2.0F, 2.0F});
    rect->scale({2.0F, 2.0F});
    rect->translate({100, 0});

    Clock clock{};
    Clock timer{};
    int fps_cnt = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        auto tp = clock.getElapsedTime().asSeconds();
        Color color{};

        color.r = 255.0 * std::abs(std::sin(tp));
        color.g = 255.0 * std::abs(std::sin(tp + std::numbers::pi / 3.0));
        color.b = 255.0 * std::abs(std::sin(tp + std::numbers::pi * 2.0 / 3.0));
        vao[0].color = color;

        color.r = color.g;
        color.g = color.b;
        color.b = 255.0 * std::abs(std::sin(tp + std::numbers::pi));
        circle->setColor(color);
        vao[1].color = color;

        color.r = color.g;
        color.g = color.b;
        color.b = 255.0 * std::abs(std::sin(tp + std::numbers::pi * 4.0 / 3.0));
        rect->setColor(color);
        vao[2].color = color;

        circle->setRadius(50.0F * (std::sin(tp * 2) / 2.0F + 1.5F));
        rect->rotate(-0.1F);
        // rect->scale({1.0001F, 1.0001F});
        rect->translate({0.1F, 0.0F});

        rect->update();
        circle->update();
        vao.update();

        shaderProgram.use();
        rect->draw();
        circle->draw();
        vao.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (timer.getElapsedTime().asSeconds() >= 1.0)
        {
            std::printf("FPS: %d\n", fps_cnt);
            fps_cnt = 0;
            timer.restart();
        }
        fps_cnt++;
    }

    shaderProgram.destroy();
    rect.reset();
    circle.reset();
    vao.destroy();

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    auto pos = rect->getPosition();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos.y -= 1.0F;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos.y += 1.0F;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos.x -= 1.0F;
        rect->setPosition(pos);
        rect->update();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos.x += 1.0F;
        rect->setPosition(pos);
        rect->update();
    }
}

void framebuffer_size_callback(GLFWwindow* /*unused*/, int width, int height)
{
    glViewport(0, 0, width, height);
}
