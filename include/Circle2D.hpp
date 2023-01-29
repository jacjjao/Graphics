#pragma once

#include "Shape.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(float radius, size_t point_count = 30);
    explicit Circle2D(Vector2f radius, size_t point_count = 30);
    ~Circle2D() override = default;

    void draw();
    void update() override;

    void setRadius(float radius);
    void setRadius(Vector2f radius);

    Circle2D(const Circle2D&)            = delete;
    Circle2D& operator=(const Circle2D&) = delete;

private:
    Vector2f m_radius;
};