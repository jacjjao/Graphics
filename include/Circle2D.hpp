#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"
#include "VertexArray.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(float radius, size_t point_count = 30) noexcept;
    ~Circle2D() override = default;

    void draw() noexcept override;
    void update() noexcept override;

    void setRadius(float radius) noexcept;

private:
    void create() noexcept;

    float m_radius;

    VertexArray   m_vao;
    ElementBuffer m_ebo;
};