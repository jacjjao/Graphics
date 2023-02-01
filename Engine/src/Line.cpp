#include "pch.hpp"
#include "../include/Line.hpp"

#include <glad/glad.h>

namespace Engine
{

    Line::Line(const Vector2f start_position, const Vector2f end_position) :
    Shape{ 2 }, 
    m_start_pos{ start_position },
    m_end_pos{ end_position },
    m_line_width{ 1.0F },
    m_color{ Color::White }
    {
        update();
    }

    void Line::setLineWidth(const float line_width)
    {
        m_line_width = line_width;
    }

    void Line::setColor(const Color color)
    {
        m_color = color;
        m_vao[0].color = m_vao[1].color = m_color;
    }

    void Line::update()
    {
        m_vao[0].position = m_start_pos;
        m_vao[1].position = m_end_pos;

        m_vao[0].color = m_vao[1].color = m_color;
        
        m_vao.update();
    }

    void Line::draw()
    {
        glCheck(glLineWidth(m_line_width));
        m_vao.draw(
            PrimitiveType::Lines,
            getTransformMatrix(),
            hasTexture() ? 0.0 : 1.0,
            m_texture
        );
        glCheck(glLineWidth(1.0f));
    }

} // namespace Engine