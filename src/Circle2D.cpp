#include "../include/Circle2D.hpp"

#include <glad/glad.h>

#include <cmath>
#include <numbers>

#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"
#include "../include/glCheck.hpp"

Circle2D::Circle2D(const float radius, const size_t point_count) noexcept :
m_radius{radius},
m_vao{point_count + 1} // including center of the circle
{
}

void Circle2D::draw() noexcept
{
    if (!m_vao.isAvailable())
    {
        create();
    }
    auto& program = ShaderProgram2D::instance();
    program.setBool("apply_texture", m_texture != nullptr);
    program.setMat3("model", getTransformMatrix());
    VertexArray::bind(m_vao);
    glCheck(glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void Circle2D::update() noexcept
{
    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto f_pi          = static_cast<float>(std::numbers::pi);
    const auto slice         = 2.0F * f_pi / f_point_count;
    const auto center        = Vector2f{Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()};
    const auto color         = getColor();

    m_vao[0].position = center;
    m_vao[0].color    = color;
    for (int i = 1; i < m_vao.size(); i++)
    {
        const auto  f_index = static_cast<float>(i);
        const float theta   = slice * f_index;
        const auto  ssin    = std::sin(theta);
        const auto  ccos    = std::cos(theta);

        Vector2f position{};
        position.x = center.x + m_radius * ssin;
        position.y = center.y - m_radius * ccos;

        Vector2f tex_coord{};
        tex_coord.x = (ccos + 1.0F) / 2.0F;
        tex_coord.y = (ssin + 1.0F) / 2.0F;

        m_vao[i].position  = position;
        m_vao[i].color     = color;
        m_vao[i].tex_coord = tex_coord;
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

void Circle2D::create() noexcept
{
    update();

    // assign texture coords
    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto f_pi          = static_cast<float>(std::numbers::pi);
    const auto slice         = 2.0F * f_pi / f_point_count;
    m_vao[0].tex_coord       = {0.5F, 0.5F};
    for (int i = 1; i < m_vao.size(); i++)
    {
        const auto  f_index = static_cast<float>(i);
        const float theta   = slice * f_index;
        const auto  ssin    = std::sin(theta);
        const auto  ccos    = std::cos(theta);

        Vector2f tex_coord{};
        tex_coord.x = (ccos + 1.0F) / 2.0F;
        tex_coord.y = (ssin + 1.0F) / 2.0F;

        m_vao[i].tex_coord = tex_coord;
    }

    const size_t          point_count = m_vao.size() - 1;
    std::vector<uint32_t> indices(3 * point_count);
    for (int i = 0, a = 1, b = 2; i < indices.size(); i += 3, a++, b++)
    {
        indices[i]     = 0;
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