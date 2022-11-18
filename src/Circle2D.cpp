#include "../include/Circle2D.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <numbers>

Circle2D::Circle2D(const glm::vec2 position, const float radius, const size_t point_count)
    : m_position{position}, m_radius{radius}, m_color{}, m_vao{point_count + 1} // including center of the circle
{
    update();
}

void Circle2D::draw()
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void Circle2D::update()
{
    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto f_pi = static_cast<float>(std::numbers::pi);
    const float slice = 2.0F * f_pi / f_point_count;
    const glm::vec2 f_position{m_position};

    m_vao[0].position = m_position;
    m_vao[0].color = m_color;
    for (int i = 1; i < m_vao.size(); i++)
    {
        const auto f_index = static_cast<float>(i);
        const float theta = slice * f_index;

        glm::vec2 position;
        position.x = f_position.x + m_radius * std::cos(theta);
        position.y = f_position.y - m_radius * std::sin(theta);

        m_vao[i].position = position;
        m_vao[i].color = m_color;
    }

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Circle2D::setColor(const Color color)
{
    m_color = color;
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