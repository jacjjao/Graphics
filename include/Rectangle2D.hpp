#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"

class Rectangle2D : public Shape
{
public:
    explicit Rectangle2D(Vector2f size) noexcept;
    ~Rectangle2D() noexcept override = default;

    Rectangle2D(const Rectangle2D&)            = delete;
    Rectangle2D& operator=(const Rectangle2D&) = delete;

    void draw() noexcept;
    void update() noexcept override;

    void setWidth(float width) noexcept;
    void setHeight(float height) noexcept;

private:
    void create() noexcept;

    Vector2f m_size;

    ElementBuffer m_ebo;
};