#pragma once

#include "VertexArray.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "ElementBuffer.hpp"
#include <vector>
#include <memory>

namespace detail 
{
struct QuadData
{
    VertexArray vao;
    ElementBuffer ebo;
};
} // detail

class Renderer2D
{
public:
    static constexpr size_t max_quad_num     = 2000;
    static constexpr size_t max_vertices_num = max_quad_num * 4;

    static void Init();

    static void begin(Camera& scene_cam);
    static void end();

    static void drawQuad(Vector2f position, Vector2f size, Color color);

private:
    static std::unique_ptr<detail::QuadData> quad_data;
    static Camera* cam;

    static size_t quad_count;
};