#include "../include/Rectangle2D.hpp"

#include <glad/glad.h>

#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"
#include "../include/glCheck.hpp"

Rectangle2D::Rectangle2D(const float width, const float height) noexcept :
m_vao{4},
m_ebo{6},
m_width{width},
m_height{height}
{
}

void Rectangle2D::draw() noexcept
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    ShaderProgram2D::instance().setMat3("model", getTransformMatrix());
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
    ShaderProgram2D::instance().setMat3("model", Matrix3::identity());
}

void Rectangle2D::update() noexcept
{
    // update position
    const auto half_width  = static_cast<float>(m_width) / 2.0F;
    const auto half_height = static_cast<float>(m_height) / 2.0F;

    const Vector2f center           = {Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()};
    const Vector2f top_right_pos    = {center.x + half_width, center.y - half_height};
    const Vector2f bottom_right_pos = {center.x + half_width, center.y + half_height};
    const Vector2f bottom_left_pos  = {center.x - half_width, center.y + half_height};
    const Vector2f top_left_pos     = {center.x - half_width, center.y - half_height};

    m_vao[0].position = top_right_pos;
    m_vao[1].position = bottom_right_pos;
    m_vao[2].position = bottom_left_pos;
    m_vao[3].position = top_left_pos;
    // update color
    const auto m_color = getColor();
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

void Rectangle2D::create() noexcept
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