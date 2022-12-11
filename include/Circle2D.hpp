#pragma once

#include "Shape.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(float radius, size_t point_count = 30) noexcept;
    ~Circle2D() noexcept override = default;

    void draw() noexcept override;
    void update() noexcept override;

    void setRadius(float radius) noexcept;

    Circle2D(const Circle2D&)            = delete;
    Circle2D& operator=(const Circle2D&) = delete;

private:
    void create() noexcept;
    void setupDraw() noexcept;

    float m_radius;
};