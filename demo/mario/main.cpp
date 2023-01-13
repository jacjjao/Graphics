// third party
#include <glad/glad.h>
#include <glfw/glfw3.h>

// standard
#include <iostream>
#include <cmath>
#include <memory>
#include <numbers>

#include "../../include/Clock.hpp"
#include "../../include/Rectangle2D.hpp"
#include "../../include/Texture.hpp"
#include "../../include/FileSystem.hpp"
#include "../../include/ShaderProgram.hpp"
#include "../../include/Camera.hpp"

void process_input(GLFWwindow* window);
void idle_for(double duration);
void release_resources();

constexpr unsigned scr_width  = 256;
constexpr unsigned scr_height = 240;
constexpr unsigned scaling_factor = 5;
constexpr double target_fps = 60.09;
constexpr Vector2f mario_size{12.0F, 16.0F};

std::unique_ptr<Texture> map_tex;
std::unique_ptr<Rectangle2D> map;
std::unique_ptr<Camera> cam;
std::unique_ptr<Rectangle2D> mario;
std::unique_ptr<Texture> mario_tex;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(scr_width * scaling_factor, scr_height * scaling_factor, "Super Mario Bros", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    Window::initialize(scr_width, scr_height);

    map_tex = std::make_unique<Texture>("C:/Users/hjaco/Desktop/mario/bg-1-1.png");

    map = std::make_unique<Rectangle2D>(map_tex->getSize());
    map->applyTexture(map_tex.get());
    map->setPosition(Vector3f{map_tex->getWidth() / 2.0F, map_tex->getHeight() / 2.0F, 0.0F}); // move it to the right position

    cam = std::make_unique<Camera>();

    mario_tex = std::make_unique<Texture>("C:/Users/hjaco/Desktop/mario/NES - Super Mario Bros - Mario & Luigi.png");
    mario = std::make_unique<Rectangle2D>(mario_size);
    mario->applyTexture(mario_tex.get());
    auto rect = mario->getTextureRect();
    rect.size = mario_size;
    rect.position.x = mario_size.x / 2.0F + 2.0F;
    rect.position.y = mario_size.y / 2.0F + 8.0F;
    mario->setTextureRect(rect);
    mario->setPosition(Vector3f(6.0F, 200.0F, 0.0F)); // so mario will in the right position


    auto& program = ShaderProgram2D::instance();
    constexpr double update_time_interval = 1.0 / target_fps;
    Clock timer{};
    while (!glfwWindowShouldClose(window))
    {
        timer.restart();
        process_input(window);
        glfwPollEvents();

        glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
        glClear(GL_COLOR_BUFFER_BIT);


        program.use();

        cam->use();

        Texture::bind(map_tex.get());
        map->draw();

        Texture::bind(mario_tex.get());
        mario->draw();


        glfwSwapBuffers(window);

        idle_for(update_time_interval - timer.getElapsedTime().asSeconds());
    }

    release_resources();

    glfwTerminate();

    return 0;
}

void process_input(GLFWwindow* window)
{
    static constexpr float move_unit = 1.0F;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mario->translate(Vector3f{-move_unit, 0.0F, 0.0F});
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mario->translate(Vector3f{move_unit, 0.0F, 0.0F});
    }

    // update camera position
    auto mario_pos = mario->getPosition();
    const auto cam_pos = cam->getPosition();
    const auto dv = mario_pos - cam_pos;
    if (dv.x > 0.0F)
    {
        cam->move(Vector3f{dv.x, 0.0F, 0.0F});
    }

    if (mario_pos.x - mario_size.x / 2.0F < cam_pos.x - scr_width / 2.0F)
    {
        mario_pos.x = cam_pos.x - scr_width / 2.0F + mario_size.x / 2.0F;
        mario->setPosition(mario_pos);
        mario->update();
    }
}

void idle_for(const double duration)
{
    static Clock timer{};
    for (timer.restart(); timer.getElapsedTime().asSeconds() < duration; );
}

void release_resources()
{
    map_tex.reset();
    map.reset();
    cam.reset();
    ShaderProgram2D::instance().destroy();
    mario_tex.reset();
    mario.reset();
}