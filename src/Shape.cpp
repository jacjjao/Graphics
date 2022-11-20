#include "../include/Shape.hpp"

#include <glm/ext/matrix_transform.hpp>

void Shape::setColor(const Color color) noexcept
{
    m_color = color;
}

Color Shape::getColor() const noexcept
{
    return m_color;
}