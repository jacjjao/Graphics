#include "../include/Shape.hpp"
#include "../include/Utility.hpp"
#include <glm/ext/matrix_transform.hpp>

Shape::Shape() : m_position{}, m_color{}
{
}

void Shape::setPosition(const glm::vec2 position) noexcept
{
    m_position = position;
}

void Shape::setColor(const Color color) noexcept
{
    m_color = color;
}

glm::vec2 Shape::getPosition() const noexcept
{
    return m_position;
}

Color Shape::getColor() const noexcept
{
    return m_color;
}

void Shape::translate(const glm::vec2 vector) noexcept
{
    auto trans_vector = Utility::vectorToOpenGL(vector);
    glm::vec3 trans{trans_vector.x, trans_vector.y, 0.0F};
    m_model = glm::translate(m_model, trans);
}

void Shape::scale(const glm::vec2 factor) noexcept
{
    glm::vec2 vector_to_center{Utility::getHalfWindowWidth() - m_position.x,
                               Utility::getHalfWindowHeight() - m_position.y};

    bool rect_pos_is_in_the_center = (vector_to_center.x == 0.0F && vector_to_center.y == 0.0F);
    if (rect_pos_is_in_the_center)
    {
        m_model = glm::scale(m_model, {factor.x, factor.y, 0.0F});
    }
    else
    {
        // ! IMPORTANT: the actual transformation order should be read in reverse
        // ! which means it's should be read from bottom to top
        translate(-vector_to_center);
        m_model = glm::scale(m_model, {factor.x, factor.y, 0.0F});
        translate(vector_to_center);
    }
}

void Shape::rotate(const float degree) noexcept
{
    glm::vec2 vector_to_center{Utility::getHalfWindowWidth() - m_position.x,
                               Utility::getHalfWindowHeight() - m_position.y};

    translate(-vector_to_center);
    m_model = glm::scale(m_model, {Utility::getHalfWindowHeight() / Utility::getHalfWindowWidth(), 1.0F, 0.0F});
    m_model = glm::rotate(m_model, glm::radians(degree), glm::vec3{0.0F, 0.0F, 1.0F});
    m_model = glm::scale(m_model, {Utility::getHalfWindowWidth() / Utility::getHalfWindowHeight(), 1.0F, 0.0F});
    translate(vector_to_center);
}