#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"

class Shape : public Drawable, public Transformable
{
public:
    explicit Shape(size_t size) noexcept;

    Shape(const Shape&)            = delete;
    Shape& operator=(const Shape&) = delete;

    virtual void update() = 0;

    void applyTexture(Texture* texture) noexcept;

    [[nodiscard]] bool hasTexture() const noexcept;

    void setColor(Color color) noexcept;

    [[nodiscard]] Color getColor() const noexcept;

protected:
    VertexArray m_vao;
    Color       m_color;
};