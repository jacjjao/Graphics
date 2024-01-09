#include "pch.hpp"
#include "include/Renderer/Renderer2D.hpp"


namespace eg
{

    std::unique_ptr<detail::QuadData> Renderer2D::quad_data = nullptr;

    eg::OrthographicCamera* Renderer2D::cam = nullptr;

    size_t Renderer2D::quad_count = 0;

    void Renderer2D::Init()
    {
        quad_data = std::make_unique<detail::QuadData>(detail::QuadData{
            VertexArray{max_vertices_num, VertexBuffer::Usage::StreamDraw},
            ElementBuffer{max_quad_num * 6}
        });

        std::vector<uint32_t> indices(max_quad_num * 6);
        uint32_t              offset = 0;
        for (size_t i = 0; i < indices.size(); i += 6, offset += 4)
        {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 0;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 2;
        }

        quad_data->ebo = std::move(indices);
        quad_data->ebo.update();
        quad_data->vao.setElementBuffer(quad_data->ebo);
    }

    void Renderer2D::begin(eg::OrthographicCamera& scene_cam)
    {
        cam = &scene_cam;

        auto& program = DefaultShaderProgram::instance();
        program.use();
        program.setMat4("view", cam->getViewMatrix());
        program.setMat4("proj", cam->getProjMatrix());
    }

    void Renderer2D::end()
    {
        if (quad_count > 0)
        {
            quad_data->vao.update();
            quad_data->vao.drawIndices(
                static_cast<int32_t>(quad_count * 6),
                PrimitiveType::Triangles
            );
            quad_count = 0;
        }
    }

    void Renderer2D::drawQuad(const float x, const float y, const float width, const float height, Color color)
    {
        if (quad_count * 6 >= max_vertices_num)
        {
            end();
        }

        Vertex bottom_left;
        bottom_left.position  = { x, y + height };
        bottom_left.tex_coord = { 0.0F, 0.0F };
        bottom_left.color = color;

        Vertex bottom_right;
        bottom_right.position = { x + width, y + height };
        bottom_right.tex_coord = { 1.0F, 0.0F };
        bottom_right.color = color;

        Vertex top_right;
        top_right.position = { x + width, y };
        top_right.tex_coord = { 1.0F, 1.0F };
        top_right.color = color;

        Vertex top_left;
        top_left.position = { x, y };
        top_left.tex_coord = { 0.0F, 1.0F };
        top_left.color = color;

        quad_data->vao[quad_count * 4 + 0] = bottom_left;
        quad_data->vao[quad_count * 4 + 1] = bottom_right;
        quad_data->vao[quad_count * 4 + 2] = top_right;
        quad_data->vao[quad_count * 4 + 3] = top_left;

        quad_count++;
    }

} // namespace eg