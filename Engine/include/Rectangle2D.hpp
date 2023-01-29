#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"

class Rectangle2D : public Shape
{
public:
    explicit Rectangle2D(Vector2f size);
    ~Rectangle2D() override = default;

    Rectangle2D(const Rectangle2D&)            = delete;
    Rectangle2D& operator=(const Rectangle2D&) = delete;

    void draw();
    void update() override;

    void setWidth(float width);
    void setHeight(float height);

private:
    void create();

    Vector2f m_size;

    ElementBuffer m_ebo;
};