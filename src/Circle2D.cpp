#include "../include/Circle2D.hpp"
#include "../include/glCheck.hpp"
#include "../include/ShaderProgram.hpp"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>
#include <numbers>

Circle2D::Circle2D(const float radius, const size_t point_count)
    : m_radius{radius}, m_vao{point_count + 1} // including center of the circle
{
    update();
}

void Circle2D::draw()
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

void Circle2D::update()
{
    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto f_pi = static_cast<float>(std::numbers::pi);
    const float slice = 2.0F * f_pi / f_point_count;
    auto pos = getPosition();
    auto color = getColor();

    m_vao[0].position = pos;
    m_vao[0].color = color;
    for (int i = 1; i < m_vao.size(); i++)
    {
        const auto f_index = static_cast<float>(i);
        const float theta = slice * f_index;

        glm::vec2 position;
        position.x = pos.x + m_radius * std::cos(theta);
        position.y = pos.y - m_radius * std::sin(theta);

        m_vao[i].position = position;
        m_vao[i].color = color;
    }

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Circle2D::setRadius(const float radius) noexcept
{
    m_radius = radius;
}

void Circle2D::create()
{
    update();

    std::vector<uint32_t> indices(3 * (m_vao.size() - 1));
    for (int i = 0, a = 1, b = 2; i < indices.size(); i += 3, a++, b++)
    {
        indices[i] = 0;
        indices[i + 1] = a;
        indices[i + 2] = (b == m_vao.size()) ? 1 : b;
    }

    m_ebo = std::move(indices);

    m_ebo.create();
    m_vao.create();

    VertexArray::bind(m_vao);
    ElementBuffer::bind(m_ebo);

    VertexArray::unbind();
    ElementBuffer::unbind();
}