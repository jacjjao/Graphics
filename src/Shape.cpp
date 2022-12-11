#include "../include/Shape.hpp"

Shape::Shape(const size_t size) noexcept : m_vao{size}, m_color{Color::White}, m_texture{nullptr}, m_tex_rect{}
{
}

void Shape::applyTexture(Texture* texture) noexcept
{
    m_texture = texture;
    if (texture != nullptr)
    {
        m_tex_rect.size     = texture->getSize();
        m_tex_rect.position = Vector2f{m_tex_rect.size.x / 2.0F, m_tex_rect.size.y / 2.0F};
    }
    else
    {
        m_tex_rect.size = m_tex_rect.position = Vector2f{0.0F, 0.0F};
    }
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