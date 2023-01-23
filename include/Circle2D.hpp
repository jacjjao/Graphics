#pragma once

#include "Shape.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(Vector2f radius, size_t point_count = 30) noexcept;
    ~Circle2D() noexcept override = default;

    void draw() noexcept;
    void update() noexcept override;

    void setRadius(Vector2f radius) noexcept;

    Circle2D(const Circle2D&)            = delete;
    Circle2D& operator=(const Circle2D&) = delete;

private:
    void create() noexcept;
    void setupDraw() noexcept;

    Vector2f m_radius;
};