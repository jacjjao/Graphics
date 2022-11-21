#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "Vertex.hpp"

class Shape : public Drawable, public Transformable
{
public:
    Shape() noexcept = default;

    virtual void update() = 0;

    void setColor(Color color) noexcept;

    [[nodiscard]] Color getColor() const noexcept;

private:
    Color m_color;
};