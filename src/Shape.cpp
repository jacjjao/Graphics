#include "../include/pch.hpp"
#include "../include/Shape.hpp"

Shape::Shape(const size_t size) noexcept :
m_vao{size},
m_color{Color::White},
m_texture{nullptr},
m_tex_rect{
    Vector2f{0.0F, 0.0F},
    Vector2f{1.0F, 1.0F}
}
{
}

void Shape::applyTexture(Texture* texture) noexcept
{
    m_texture = texture;
}

bool Shape::hasTexture() const noexcept
{
    return (m_texture != nullptr);
}

void Shape::setColor(const Color color) noexcept
{
    m_color = color;
}

Color Shape::getColor() const noexcept
{
    return m_color;
}

void Shape::setTextureRect(const Rect rect) noexcept
{
    m_tex_rect = rect;
}

Rect Shape::getTextureRect() const noexcept
{
    return m_tex_rect;
}

bool Shape::isCreated() const noexcept
{
    return m_is_created;
}
