#include "../include/Shape.hpp"

Shape::Shape(const size_t size) noexcept : m_vao{size}, m_color{}
{
}

void Shape::applyTexture(Texture* texture) noexcept
{
    m_vao.applyTexture(texture);
}

bool Shape::hasTexture() const noexcept
{
    return m_vao.hasTexture();
}

void Shape::setColor(const Color color) noexcept
{
    m_color = color;
}

Color Shape::getColor() const noexcept
{
    return m_color;
}