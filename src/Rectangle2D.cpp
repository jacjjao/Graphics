#include "../include/Rectangle2D.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

Rectangle2D::Rectangle2D(const float width, const float height) : m_vao{4}, m_ebo{6}, m_width{width}, m_height{height}
{
    update();
}

void Rectangle2D::draw()
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    ShaderProgram2D::instance().setMat4("model", getTransformMatrix());
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
    ShaderProgram2D::instance().setMat4("model", glm::identity<glm::mat4>());
}

void Rectangle2D::update()
{
    // update position
    const auto half_width = static_cast<float>(m_width) / 2.0F;
    const auto half_height = static_cast<float>(m_height) / 2.0F;
    const auto pos = getPosition();

    m_vao[0].position = {pos.x + half_width, pos.y - half_height}; // top right
    m_vao[1].position = {pos.x + half_width, pos.y + half_height}; // bottom right
    m_vao[2].position = {pos.x - half_width, pos.y + half_height}; // bottom left
    m_vao[3].position = {pos.x - half_width, pos.y - half_height}; // top left
    // update color
    auto m_color = getColor();
    for (auto& [_, color] : m_vao)
    {
        color = m_color;
    }

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Rectangle2D::setWidth(const float width) noexcept
{
    m_width = width;
}

void Rectangle2D::setHeight(const float height) noexcept
{
    m_height = height;
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