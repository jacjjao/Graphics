#pragma once

#include "Shape.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(float radius, size_t point_count = 30) noexcept;
    explicit Circle2D(Vector2f radius, size_t point_count = 30) noexcept;
    ~Circle2D() noexcept override = default;

    void draw() noexcept;
    void update() noexcept override;

    void setRadius(float radius) noexcept;
    void setRadius(Vector2f radius) noexcept;

    Circle2D(const Circle2D&)            = delete;
    Circle2D& operator=(const Circle2D&) = delete;

private:
    Vector2f m_radius;
};