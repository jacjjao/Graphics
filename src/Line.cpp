#include "../include/pch.hpp"
#include "../include/Line.hpp"

#include <glad/glad.h>

Line::Line(const Vector2f start_position, const Vector2f end_position) :
m_start_pos{start_position},
m_end_pos{end_position},
m_vao{2},
m_line_width{1.0F},
m_color{Color::White}
{
    m_vao[0].position = m_start_pos;
    m_vao[1].position = m_end_pos;

    m_vao[0].color = m_vao[1].color = m_color;
}

void Line::setLineWidth(const float line_width) noexcept
{
    m_line_width = line_width;
}

void Line::setColor(const Color color) noexcept
{
    m_color        = color;
    m_vao[0].color = m_vao[1].color = m_color;
}

void Line::draw() noexcept
{
    glCheck(glLineWidth(m_line_width));
    m_vao.draw(PrimitiveType::Lines);
}
