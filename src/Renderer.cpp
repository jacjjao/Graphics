#include "../include/pch.hpp"
#include "../include/Renderer.hpp"
#include "../include/ShaderProgram.hpp"

Renderer::Renderer(unsigned scene_width, unsigned scene_height) : 
m_camera(
    -scene_width / 2, 
    scene_width / 2, 
    -scene_height / 2, 
    scene_height / 2),
m_drawables{}
{
}

void Renderer::add(Drawable* drawable) noexcept
{
    drawable->create();
    m_drawables.emplace_back(drawable);
}

void Renderer::drawAll() noexcept
{
    static auto& program = DefaultShaderProgram::instance();
    program.use();
    program.setMat4("view", m_camera.getViewMatrix());
    program.setMat4("proj", m_camera.getProjMatrix());
    for (auto& drawable : m_drawables)
    {
        drawable->draw();
    }
}