#pragma once

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "Shape.hpp"

class Circle2D : public Shape
{
public:
    explicit Circle2D(float radius, size_t point_count = 30);

    void draw() override;
    void update() override;

    void setRadius(float radius) noexcept;

private:
    void create();

    float m_radius;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};