#include "../include/Rectangle2D.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/glCheck.hpp"
#include "../include/Utility.hpp"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/matrix.hpp>

Rectangle2D::Rectangle2D(const float width, const float height)
    : m_vao{4}, m_ebo{6}, m_position{}, m_width{width}, m_height{height}, m_color{}, m_model{1.0F}
{
    update();
}

void Rectangle2D::setColor(const Color color) noexcept
{
    m_color = color;
}

void Rectangle2D::setPosition(const glm::vec2 position) noexcept
{
    m_position = position;
}

void Rectangle2D::setWidth(const float width) noexcept
{
    m_width = width;
}

void Rectangle2D::setHeight(const float height) noexcept
{
    m_height = height;
}

void Rectangle2D::draw()
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    ShaderProgram2D::instance().setMat4("model", m_model);
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void Rectangle2D::update()
{
    // update position
    const auto half_width = static_cast<float>(m_width) / 2.0F;
    const auto half_height = static_cast<float>(m_height) / 2.0F;
    m_vao[0].position = {m_position.x + half_width, m_position.y - half_height}; // top right
    m_vao[1].position = {m_position.x + half_width, m_position.y + half_height}; // bottom right
    m_vao[2].position = {m_position.x - half_width, m_position.y + half_height}; // bottom left
    m_vao[3].position = {m_position.x - half_width, m_position.y - half_height}; // top left
    // update color
    for (auto& [_, color] : m_vao)
    {
        color = m_color;
    }

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Rectangle2D::create()
{
    update();

    m_vao.create();
    VertexArray::bind(m_vao);

    m_ebo = {0, 1, 3, 1, 2, 3};
    m_ebo.create();
    ElementBuffer::bind(m_ebo);

    VertexArray::unbind();
    ElementBuffer::unbind();
}

glm::vec2 Rectangle2D::getPosition() const noexcept
{
    return m_position;
}

void Rectangle2D::translate(const glm::vec2 vector) noexcept
{
    auto trans_vector = Utility::vectorToOpenGL(vector);
    glm::vec3 trans{trans_vector.x, trans_vector.y, 0.0F};
    m_model = glm::translate(m_model, trans);
}

void Rectangle2D::scale(const glm::vec2 factor) noexcept
{
    glm::vec2 vector_to_center{Utility::getHalfWindowWidth() - m_position.x,
                               Utility::getHalfWindowHeight() - m_position.y};
    const auto vector_to_center_gl = Utility::vectorToOpenGL(vector_to_center);

    // ! IMPORTANT: the actual transformation order should be read in reverse
    // ! which means it's should read from bottom to top
    m_model = glm::translate(m_model, {-vector_to_center_gl.x, -vector_to_center_gl.y, 0.0F});
    m_model = glm::scale(m_model, {factor.x, factor.y, 0.0F});
    m_model = glm::translate(m_model, {vector_to_center_gl.x, vector_to_center_gl.y, 0.0F});
}

void Rectangle2D::rotate(const float degree) noexcept
{
    glm::vec2 vector_to_center{Utility::getHalfWindowWidth() - m_position.x,
                               Utility::getHalfWindowHeight() - m_position.y};
    const auto vector_to_center_gl = Utility::vectorToOpenGL(vector_to_center);

    m_model = glm::translate(m_model, {-vector_to_center_gl.x, -vector_to_center_gl.y, 0.0F});
    m_model = glm::rotate(m_model, glm::radians(degree), glm::vec3{0.0F, 0.0F, 1.0F});
    m_model = glm::translate(m_model, {vector_to_center_gl.x, vector_to_center_gl.y, 0.0F});
}