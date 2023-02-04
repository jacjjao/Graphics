#pragma once

#include "Shape.hpp"

namespace Engine
{

    class Circle2D : public Shape
    {
    public:
        explicit Circle2D(float radius, size_t point_count = 30);
        explicit Circle2D(Vector2f radius, size_t point_count = 30);
        ~Circle2D() override = default;

        void draw() override;
        void update() override;

        void setRadius(float radius) { m_radius.x = m_radius.y = radius; }
        void setRadius(Vector2f radius) { m_radius = radius; }

        Circle2D(Circle2D&&) = default;
    private:
        Vector2f m_radius;
    };

} // namespace Engine