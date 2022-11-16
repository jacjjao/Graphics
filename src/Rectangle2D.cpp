#include "../include/Rectangle2D.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>

Rectangle2D::Rectangle2D(const glm::vec2 position, const uint32_t width, const uint32_t height)
    : m_vao{4}, m_ebo{6}, m_position{position}, m_width{width}, m_height{height}, m_color{}
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

void Rectangle2D::setWidth(const uint32_t width) noexcept
{
    m_width = width;
}

void Rectangle2D::setHeight(const uint32_t height) noexcept
{
    m_height = height;
}

void Rectangle2D::draw()
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void Rectangle2D::update()
{
    // update position
    m_vao[0].position = {m_position.x + m_width, m_position.y};
    m_vao[1].position = {m_position.x + m_width, m_position.y + m_height};
    m_vao[2].position = {m_position.x, m_position.y + m_height};
    m_vao[3].position = m_position;
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