#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"
#include "VertexArray.hpp"

class Rectangle2D : public Shape
{
public:
    explicit Rectangle2D(float width, float height) noexcept;
    ~Rectangle2D() noexcept override = default;

    Rectangle2D(const Rectangle2D&)            = delete;
    Rectangle2D& operator=(const Rectangle2D&) = delete;

    void draw() noexcept override;
    void update() noexcept override;

    void setWidth(float width) noexcept;
    void setHeight(float height) noexcept;

private:
    void create() noexcept;

    float m_width, m_height;

    VertexArray   m_vao;
    ElementBuffer m_ebo;
};