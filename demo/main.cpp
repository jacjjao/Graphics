// third party
#include <glad/glad.h>
#include <glfw/glfw3.h>
// standard
#include <iostream>
#include <cmath>
#include <memory>
#include <numbers>

#include "../include/Circle2D.hpp"
#include "../include/Clock.hpp"
#include "../include/Rectangle2D.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"
#include "../include/Vector.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH  = 2000;
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

    {
        auto& shaderProgram = ShaderProgram2D::instance();

        rect = std::make_unique<Rectangle2D>(100, 100);
        rect->setPosition({400, 900});

        Circle2D circle{50.0F};
        circle.setPosition({1700, 900});

        VertexArray vao{3};
        vao[0].position = {1000.0, 100.0};
        vao[1].position = {static_cast<float>(1000.0 + 500.0 * std::sqrt(3.0) / 2.0), 850.0};
        vao[2].position = {static_cast<float>(1000.0 - 500.0 * std::sqrt(3.0) / 2.0), 850.0};

        circle.scale({2.0F, 2.0F});
        rect->scale({2.0F, 2.0F});
        rect->translate({100, 0});

        Clock clock{};
        Clock timer{};
        int   fps_cnt = 0;
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
            glClear(GL_COLOR_BUFFER_BIT);

            auto  tp = clock.getElapsedTime().asSeconds();
            Color color{};

            color.r      = 255.0 * std::abs(std::sin(tp));
            color.g      = 255.0 * std::abs(std::sin(tp + std::numbers::pi / 3.0));
            color.b      = 255.0 * std::abs(std::sin(tp + std::numbers::pi * 2.0 / 3.0));
            vao[0].color = color;

            color.r = color.g;
            color.g = color.b;
            color.b = 255.0 * std::abs(std::sin(tp + std::numbers::pi));
            circle.setColor(color);
            vao[1].color = color;

            color.r = color.g;
            color.g = color.b;
            color.b = 255.0 * std::abs(std::sin(tp + std::numbers::pi * 4.0 / 3.0));
            rect->setColor(color);
            vao[2].color = color;

            float factor = std::sin(tp * 2) / 2.0F + 1.5F;
            circle.scale({factor, factor});
            rect->rotate(-0.05F);

            rect->update();
            circle.update();
            vao.update();

            shaderProgram.use();
            rect->draw();
            circle.draw();
            vao.draw();

            glfwSwapBuffers(window);
            glfwPollEvents();

            if (auto tp = timer.getElapsedTime().asSeconds(); tp >= 1.0)
            {
                std::cout << "FPS: " << static_cast<double>(fps_cnt) / tp << '\n';
                fps_cnt = 0;
                timer.restart();
            }
            fps_cnt++;
        }

        shaderProgram.destroy();
        rect.reset();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        rect->translate({0.0F, -1.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        rect->translate({0.0F, 1.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        rect->translate({-1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        rect->translate({1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        rect->setWidth(50.0F);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        rect->setWidth(100.0F);
    }
}

void framebuffer_size_callback(GLFWwindow* /*unused*/, int width, int height)
{
    glViewport(0, 0, width, height);
}
