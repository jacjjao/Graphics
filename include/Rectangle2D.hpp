#pragma once

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "Shape.hpp"

class Rectangle2D : public Shape
{
public:
    explicit Rectangle2D(float width, float height);

    Rectangle2D(const Rectangle2D&) = delete;
    Rectangle2D& operator=(const Rectangle2D&) = delete;

    void draw() override;
    void update() override;

    void setWidth(float width) noexcept;
    void setHeight(float height) noexcept;

private:
    void create();

    float m_width, m_height;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};