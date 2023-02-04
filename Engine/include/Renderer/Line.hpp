#pragma once

#include "Shape.hpp"

namespace Engine
{

    class Line : public Shape
    {
    public:
        Line(Vector2f start_position, Vector2f end_position);

        void setLineWidth(float line_width) { m_line_width = line_width; }
        void setColor(Color color) { m_color = color; }

        void update() override;
        void draw() override;

        Line(const Line&) = delete;
        Line& operator=(const Line&) = delete;

    private:

        Vector2f m_start_pos, m_end_pos;

        float m_line_width;

        Color m_color;
    };

} // namespace Engine 