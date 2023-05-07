#include "pch.hpp"
#include "include/Renderer/Rectangle2D.hpp"
#include "include/Renderer/ShaderProgram.hpp"

#include <glad/glad.h>

namespace eg
{

    Rectangle2D::Rectangle2D() :
        Shape{ 4 },
        m_ebo{ 6 }
    {
        m_ebo = { 0, 1, 3, 1, 2, 3 };
        m_ebo.update();
        m_vao.setElementBuffer(m_ebo);
    }

    Rectangle2D::Rectangle2D(const float width, const float height) :
        Rectangle2D()
    {
        m_width = width;
        m_height = height;
        m_ebo = { 0, 1, 3, 1, 2, 3 };
        m_ebo.update();
        m_vao.setElementBuffer(m_ebo);
        update();
    }

    void Rectangle2D::draw()
    {
        m_vao.drawIndices(
            m_ebo.size(),
            PrimitiveType::Triangles,
            getTransformMatrix(),
            hasTexture() ? 0.0F : 1.0F,
            m_texture.get()
        );
    }

    void Rectangle2D::update()
    {
        // update size
        const float half_width = m_width / 2.0f;
        const float half_height = m_height / 2.0f;
        const auto [x, y] = getPosition();

        setOrigin({ x, y });

        const Vector2f top_right_pos{ x + half_width, y + half_height };
        const Vector2f bottom_right_pos{ x + half_width, y - half_height };
        const Vector2f bottom_left_pos{ x - half_width, y - half_height };
        const Vector2f top_left_pos{ x - half_width, y + half_height };

        m_vao[0].position = top_right_pos;
        m_vao[1].position = bottom_right_pos;
        m_vao[2].position = bottom_left_pos;
        m_vao[3].position = top_left_pos;
        // update color
        for (auto& vertex : m_vao)
        {
            vertex.color = m_color;
        }
        // update texture coordinate
        if (hasTexture())
        {
            m_vao[0].tex_coord = m_tex_rect.position + m_tex_rect.size;
            m_vao[1].tex_coord = { m_tex_rect.position.x + m_tex_rect.size.x, m_tex_rect.position.y };
            m_vao[2].tex_coord = m_tex_rect.position;
            m_vao[3].tex_coord = { m_tex_rect.position.x, m_tex_rect.position.y + m_tex_rect.size.y };
        }

        m_vao.update();
    }

} // namespace eg