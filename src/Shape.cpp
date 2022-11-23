#include "../include/Shape.hpp"

void Shape::setColor(const Color color) noexcept
{
    m_color = color;
}

void Shape::applyTexture(Texture* texture) noexcept
{
    m_texture = texture;
}

Color Shape::getColor() const noexcept
{
    return m_color;
}

bool Shape::hasTexture() const noexcept
{
    return m_texture != nullptr;
}