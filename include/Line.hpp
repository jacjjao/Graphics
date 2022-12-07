#pragma once

#include "VertexArray.hpp"

class Line : public Drawable
{
public:
    Line(Vector2f start_position, Vector2f end_position);

    void draw() noexcept override;

private:
    Vector2f    m_start_pos, m_end_pos;
    VertexArray m_vao;
};