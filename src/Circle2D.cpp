#include "../include/Circle2D.hpp"

#include <glad/glad.h>

#include <cmath>
#include <numbers>

#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"
#include "../include/glCheck.hpp"

Circle2D::Circle2D(const float radius, const size_t point_count) noexcept :
m_radius{radius},
m_vao{point_count + 2} // including center of the circle
{
}

void Circle2D::draw() noexcept
{
    if (!m_vao.isAvailable())
    {
        create();
    }

    setupDraw();
    glCheck(glDrawArrays(GL_TRIANGLE_FAN, 0, m_vao.size()));
    cleanUpDraw();
}

void Circle2D::update() noexcept
{
    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto f_pi          = static_cast<float>(std::numbers::pi);
    const auto slice         = 2.0F * f_pi / f_point_count;
    const auto center        = Vector2f{Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()};

    m_vao[0].position = center;
    m_vao[0].color    = m_color;
    for (int i = 1; i < m_vao.size() - 1; i++)
    {
        const auto  f_index = static_cast<float>(i);
        const float theta   = slice * f_index;
        const auto  ssin    = std::sin(theta);
        const auto  ccos    = std::cos(theta);

        Vector2f position{};
        position.x = center.x + m_radius * ssin;
        position.y = center.y - m_radius * ccos;

        m_vao[i].position = position;
        m_vao[i].color    = m_color;
    }
    m_vao.back() = m_vao[1];


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

    m_vao.create();
}

void Circle2D::setupDraw() noexcept
{
    if (m_texture != nullptr)
    {
        glCheck(glActiveTexture(GL_TEXTURE0));
        Texture::bind(*m_texture);
    }
    auto& program = ShaderProgram2D::instance();
    program.setBool("apply_texture", hasTexture());
    program.setMat3("model", getTransformMatrix());
    VertexArray::bind(m_vao);
}

void Circle2D::cleanUpDraw() noexcept
{
    VertexArray::unbind();
    Texture::unbind();
}