#include "../include/pch.hpp"
#include "../include/Graphics.hpp"

// third party
#include <glad/glad.h>
#include <glfw/glfw3.h>

// standard
#include <iostream>
#include <cmath>
#include <memory>
#include <numbers>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void GLAPIENTRY dbg_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << message << '\n';
}

constexpr unsigned SCR_WIDTH  = 2000;
constexpr unsigned SCR_HEIGHT = 1200;

std::unique_ptr<Rectangle2D> rect{};
std::unique_ptr<Circle2D>    circle{};
std::unique_ptr<Texture>     texture{};
std::unique_ptr<Camera>      camera{};

constexpr auto scr_half_width  = static_cast<float>(SCR_WIDTH / 2);
constexpr auto scr_half_height = static_cast<float>(SCR_HEIGHT / 2);

constexpr Vector2f screenPointToNDC(const Vector2f point) noexcept
{
    Vector2f result{0.0F, 0.0F};

    result.x = (point.x - scr_half_width);
    result.y = (scr_half_height - point.y);

    return result;
}

constexpr Vector3f screenPointToNDC(const Vector3f point) noexcept
{
    Vector3f result{0.0F, 0.0F, 0.0F};

    result.x = (point.x - scr_half_width);
    result.y = (scr_half_height - point.y);

    return result;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialized GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /*
#if(DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    */
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
    /*
#if(DEBUG)
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(dbg_callback, nullptr);
#endif
   */ 
    Texture::Init();
    Renderer2D::Init();
    TextRenderer::initialize(48, SCR_WIDTH, SCR_HEIGHT);

    {
        camera = std::make_unique<Camera>(-scr_half_width, scr_half_width, -scr_half_height, scr_half_height);

        auto& shaderProgram = DefaultShaderProgram::instance();
        
        texture = std::make_unique<Texture>(FileSystem::getPath("/asset/images/container.jpg"));
        Texture texture2{FileSystem::getPath("/asset/images/awesomeface.png")};
        Texture texture3{};
        uint32_t data = 0xffffffff;
        texture3.createFromData(&data, 1, 1);

        rect = std::make_unique<Rectangle2D>(Vector2f{100, 100});
        rect->setPosition(screenPointToNDC(Vector3f{300, 1000, 0.0F}));
        rect->applyTexture(&texture3);


        auto tex_rect = rect->getTextureRect();
        tex_rect.size.x /= 2.0F;
        tex_rect.size.y /= 2.0F;
        tex_rect.position.x /= 2.0F;
        tex_rect.position.y /= 2.0F;
        rect->setTextureRect(tex_rect);
        
        circle = std::make_unique<Circle2D>(50.0F);
        circle->setPosition(screenPointToNDC({1800.0F, 1000.0F, 0.0F}));
        circle->applyTexture(&texture2);
        
        VertexArray vao{3};
        vao[0].position = screenPointToNDC({1000.0, 100.0});
        vao[1].position = screenPointToNDC({static_cast<float>(1000.0 + 500.0 * std::sqrt(3.0) / 2.0), 850.0});
        vao[2].position = screenPointToNDC({static_cast<float>(1000.0 - 500.0 * std::sqrt(3.0) / 2.0), 850.0});
        vao.setUsage(VertexBuffer::Usage::StreamDraw);

        circle->scale({2.0F});
        rect->scale({2.0F});
        
        Line line{screenPointToNDC({100.0F, 100.0F}), screenPointToNDC({500.0F, 100.0F})};
        line.setLineWidth(10.0F);
        line.setColor(Color{255, 127, 127});
        
        Clock clock{};
        Clock timer{};
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
            circle->scale({factor, factor});
            rect->rotate(-0.05F);

            rect->update();
            circle->update();
            vao.update();

            Texture::bind(texture.get(), 1);
            Texture::bind(&texture2, 10);
            Texture::bind(&texture3, 0);

            shaderProgram.use();
            
            camera->use();
            rect->draw();
            circle->draw();
            vao.draw();
            line.draw();
            constexpr auto text_pos = screenPointToNDC({0, 0});
            constexpr auto text_color = Color::fromHex(0x11F311FF);

            Renderer2D::begin(*camera);
            Renderer2D::drawQuad({ 100, 100 }, { 100, 100 }, Color::Red);
            Renderer2D::drawQuad({ -100, 100 }, { 100, 100 }, Color::Blue);
            Renderer2D::end();
            
            TextRenderer::renderText("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                     text_pos,
                                     text_color,
                                     48);
                                     
            glfwSwapBuffers(window);

            while (timer.getElapsedTime().asMilliseconds() < 1)
            {
            }
            timer.restart();
        }
    }

    TextRenderer::releaseResources();

    glfwTerminate();

#if(DEBUG)
    std::cout << "\nThe context is free ignore the error down below\n\n";
#endif

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
        // rect->translate({0.0F, 1.0F, 0.0F});
        camera->move({0.0F, 1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // rect->translate({0.0F, -1.0F, 0.0F});
        camera->move({0.0F, -1.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // rect->translate({-1.0F, 0.0F, 0.0F});
        camera->move({-1.0F, 0.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // rect->translate({1.0F, 0.0F, 0.0F});d
        camera->move({1.0F, 0.0F, 0.0F});
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
    static Vector2f scale{1.0F};
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
