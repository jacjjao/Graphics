#pragma once

#include "VertexArray.hpp"

class Line : public Drawable
{
public:
    Line(Vector2f start_position, Vector2f end_position);

    void setLineWidth(float line_width) noexcept;
    void setColor(Color color) noexcept;

    void draw() noexcept override;

    Line(const Line&)            = delete;
    Line& operator=(const Line&) = delete;

private:
    Vector2f m_start_pos, m_end_pos;

    float m_line_width;

    Color m_color;

    VertexArray m_vao;
};