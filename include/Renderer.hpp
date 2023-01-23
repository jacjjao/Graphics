#pragma once

#include "Drawable.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include <vector>
#include <memory>

class Renderer
{
public:
    Renderer(unsigned scene_width, unsigned scene_height);

    void add(Drawable* drawable) noexcept;
    void drawAll() noexcept;

private:
    std::vector<std::unique_ptr<Drawable>> m_drawables;

    Camera m_camera;
};