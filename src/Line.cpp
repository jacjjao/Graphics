#include "../include/Line.hpp"


Line::Line(const Vector2f start_position, const Vector2f end_position) :
m_start_pos{start_position},
m_end_pos{end_position},
m_vao{2}
{
    m_vao[0].position = m_start_pos;
    m_vao[1].position = m_end_pos;
}

void Line::draw() noexcept
{
    m_vao.draw(PrimitiveType::Lines);
}