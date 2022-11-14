#include "../include/Rectangle2D.hpp"
#include "../include/glCheck.hpp"
#include "../include/Utility.hpp"
#include <glad/glad.h>

Rectangle2D::Rectangle2D(const glm::ivec2 position, const uint32_t width, const uint32_t height)
    : m_vao{4}, m_ebo{6}, m_pos{position}, m_width{width}, m_height{height}, m_color{}
{
}

void Rectangle2D::setColor(const Color color)
{
    m_color = color;
}

void Rectangle2D::setPosition(const glm::ivec2 position)
{
    m_pos = position;
}

void Rectangle2D::setWidth(const uint32_t width)
{
    m_width = width;
}

void Rectangle2D::setHeight(const uint32_t height)
{
    m_height = height;
}

void Rectangle2D::draw()
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    m_vao.bind();
    glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void Rectangle2D::update()
{
    if (m_vao.isAvailable())
    {
        updateVboData();
        m_vao.update();
    }
}

void Rectangle2D::updateVboData()
{
    // update position
    auto pos_on_gl = util::pointToOpenGL({m_pos.x + m_width, m_pos.y});
    m_vao[0].position = pos_on_gl;

    pos_on_gl = util::pointToOpenGL({m_pos.x + m_width, m_pos.y + m_height});
    m_vao[1].position = pos_on_gl;

    pos_on_gl = util::pointToOpenGL({m_pos.x, m_pos.y + m_height});
    m_vao[2].position = pos_on_gl;

    pos_on_gl = util::pointToOpenGL(m_pos);
    m_vao[3].position = pos_on_gl;

    // update color
    for (auto& [_, color] : m_vao)
    {
        color = m_color;
    }
}

void Rectangle2D::create()
{
    updateVboData();

    m_vao.create();
    m_vao.bind();

    m_ebo = {0, 1, 3, 1, 2, 3};
    m_ebo.create();
    m_ebo.bind();

    VertexBuffer::unbind();
    VertexArray::unbind();
    ElementBuffer::unbind();
}

glm::ivec2 Rectangle2D::getPosition() const noexcept
{
    return m_pos;
}