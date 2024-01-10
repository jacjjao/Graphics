#pragma once

#include "VertexArray.hpp"
#include "OrthographicCamera.hpp"
#include "ShaderProgram.hpp"
#include "ElementBuffer.hpp"
#include <vector>
#include <memory>


namespace eg
{

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
        static constexpr size_t max_quad_num = 10000;
        static constexpr size_t max_vertices_num = max_quad_num * 4;

        static void Init();

        static void begin(eg::OrthographicCamera& scene_cam);
        static void end();

        static void drawQuad(float x, float y, float width, float height, Color color);
        static void drawQuad(float x, float y, float width, float height, Color color, float degree);

        static void drawCircle(float x, float y, float radius);

    private:
        static constexpr int quad_indix_count = 6;

        static std::unique_ptr<detail::QuadData> quad_data;
        static eg::OrthographicCamera* cam;

        static size_t quad_count;
    };

} // namespace eg