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
#include "../include/Window.hpp"
#include "../include/Vector.hpp"
#include "../include/Texture.hpp"
#include "../include/FileSystem.hpp"
#include "../include/Camera.hpp"
#include "../include/Line.hpp"
#include "../include/TextRenderer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH  = 2000;
const unsigned int SCR_HEIGHT = 1200;

std::unique_ptr<Rectangle2D> rect{};
std::unique_ptr<Circle2D>    circle{};
std::unique_ptr<Texture>     texture{};
std::unique_ptr<Camera>      camera{};

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
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    Window::initialize(SCR_WIDTH, SCR_HEIGHT);
    TextRenderer::initialize();

    {
        camera = std::make_unique<Camera>();

        auto& shaderProgram = ShaderProgram2D::instance();

        texture = std::make_unique<Texture>(FileSystem::getPath("/asset/images/container.jpg"));

        rect = std::make_unique<Rectangle2D>(Vector2f{100, 100});
        rect->setPosition(Vector3f{300, 1000, 0});

        rect->applyTexture(texture.get());

        auto tex_rect = rect->getTextureRect();
        tex_rect.size.x /= 2.0F;
        tex_rect.size.y /= 2.0F;
        tex_rect.position.x /= 2.0F;
        tex_rect.position.y /= 2.0F;
        // rect->setTextureRect(tex_rect);

        circle = std::make_unique<Circle2D>(50.0F);
        circle->setPosition(Vector3f{1800.0F, 1000.0F, 0});

        circle->applyTexture(texture.get());

        circle->setTextureRect(tex_rect);

        VertexArray vao{3};
        vao[0].position = Vector2f{1000.0, 100.0};
        vao[1].position = Vector2f{static_cast<float>(1000.0 + 500.0 * std::sqrt(3.0) / 2.0), 850.0};
        vao[2].position = Vector2f{static_cast<float>(1000.0 - 500.0 * std::sqrt(3.0) / 2.0), 850.0};
        vao.setUsage(VertexBuffer::Usage::StreamDraw);

        circle->scale(Vector2f{2.0F, 2.0F});
        rect->scale(Vector2f{2.0F, 2.0F});
        rect->translate(Vector3f{100, 0, 0});

        Line line{Vector2f{100.0F, 100.0F}, Vector2f{500.0F, 100.0F}};
        line.setLineWidth(10.0F);
        line.setColor(Color{255, 127, 127});

        Clock clock{};
        Clock timer{};
        int   fps_cnt = 0;
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            glfwPollEvents();

            glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.use();

            auto  tp = clock.getElapsedTime().asSeconds();
            Color color{};

            color.r      = 255.0 * std::abs(std::sin(tp));
            color.g      = 255.0 * std::abs(std::sin(tp + std::numbers::pi / 3.0));
            color.b      = 255.0 * std::abs(std::sin(tp + std::numbers::pi * 2.0 / 3.0));
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

            const float factor = std::sin(tp * 2) / 2.0F + 1.5F;
            circle->scale(Vector2f{factor, factor});
            rect->rotate(-0.05F);

            rect->update();
            circle->update();
            vao.update();

            Texture::bind(texture.get());

            camera->use();
            rect->draw();
            circle->draw();
            vao.draw();
            line.draw();

            Texture::unbind();

            TextRenderer::renderText("Hello World", 0.0F, 0.0F, Color::White, 48);

            glfwSwapBuffers(window);

            if (auto tp = timer.getElapsedTime().asSeconds(); tp >= 1.0)
            {
                std::printf("FPS: %.2lf\n", static_cast<double>(fps_cnt) / tp);
                fps_cnt = 0;
                timer.restart();
            }
            fps_cnt++;
        }
    }

    glfwTerminate();
    std::cout << "\nThe context is free ignore the error down below\n\n";

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
        // rect->translate({0.0F, -1.0F});
        camera->move(Vector3f{0.0F, -1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // rect->translate({0.0F, 1.0F});
        camera->move(Vector3f{0.0F, 1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // rect->translate({-1.0F, 0.0F});
        camera->move(Vector3f{-1.0F, 0.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // rect->translate({1.0F, 0.0F});
        camera->move(Vector3f{1.0F, 0.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        // rect->translate({1.0F, 0.0F});
        camera->rotate(-0.1F);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        // rect->translate({1.0F, 0.0F});
        camera->rotate(0.1F);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        circle->applyTexture(nullptr);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        circle->applyTexture(texture.get());
    }
}

void scrollCallback(GLFWwindow* window, double /* xoffset */, double yoffset)
{
    static Vector2f scale{1.0F, 1.0F};
    if (yoffset > 0)
    {
        scale.x *= 1.1F;
        scale.y *= 1.1F;
    }
    else
    {
        scale.x *= 0.9F;
        scale.y *= 0.9F;
    }
    camera->scale(scale);
}

void framebuffer_size_callback(GLFWwindow* /*unused*/, int width, int height)
{
    glViewport(0, 0, width, height);
}
