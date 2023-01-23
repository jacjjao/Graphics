#pragma once

#include "Transformable.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Rect.hpp"

class Shape : public Transformable
{
public:
    explicit Shape(size_t size) noexcept;
    virtual ~Shape() = default;

    Shape(const Shape&)            = delete;
    Shape& operator=(const Shape&) = delete;

    virtual void update() = 0;

    void applyTexture(Texture* texture) noexcept;

    [[nodiscard]] bool hasTexture() const noexcept;

    void setColor(Color color) noexcept;

    [[nodiscard]] Color getColor() const noexcept;

    void setTextureRect(Rect rect) noexcept;

    [[nodiscard]] Rect getTextureRect() const noexcept;

protected:
    VertexArray m_vao;

    Color m_color;

    Texture* m_texture;

    Rect m_tex_rect;
};