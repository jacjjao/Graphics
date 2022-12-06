#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"
#include "../include/glCheck.hpp"
#include "../include/Circle2D.hpp"

#include <glad/glad.h>

#include <cmath>
#include <numbers>

Circle2D::Circle2D(const float radius, const size_t point_count) noexcept : Shape(point_count), m_radius{radius}
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
    const auto f_point_count   = static_cast<float>(m_vao.size() - 1);
    const auto f_pi            = static_cast<float>(std::numbers::pi);
    const auto slice           = 2.0F * f_pi / f_point_count;
    const auto center          = Vector2f{Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()};
    const auto color           = getColor();
    const auto half_tex_width  = (hasTexture()) ? m_vao.getTexture()->getWidth() / 2.0F : 0.0F;
    const auto half_tex_height = (hasTexture()) ? m_vao.getTexture()->getHeight() / 2.0F : 0.0F;

    m_vao[0].position  = center;
    m_vao[0].color     = color;
    m_vao[0].tex_coord = {half_tex_width, half_tex_height};
    for (int i = 1; i < m_vao.size() - 1; i++)
    {
        const auto  f_index = static_cast<float>(i);
        const float theta   = slice * f_index;
        const auto  ssin    = std::sin(theta);
        const auto  ccos    = std::cos(theta);

        Vector2f position{};
        position.x = center.x + m_radius * ssin;
        position.y = center.y - m_radius * ccos;

        Vector2f tex_coord{};
        tex_coord.x = half_tex_width + half_tex_width * ccos;
        tex_coord.y = half_tex_height + half_tex_height * ssin;

        m_vao[i].position  = position;
        m_vao[i].color     = color;
        m_vao[i].tex_coord = tex_coord;
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

    m_vao.create();
}

void Circle2D::setupDraw() noexcept
{
    if (hasTexture())
    {
        glCheck(glActiveTexture(GL_TEXTURE0));
        Texture::bind(m_vao.getTexture());
    }

    auto& program = ShaderProgram2D::instance();

    program.setFloat("color_alpha", hasTexture() ? 0.0F : 1.0F);
    program.setMat3("model", getTransformMatrix());

    VertexArray::bind(&m_vao);
}

void Circle2D::cleanUpDraw() noexcept
{
    VertexArray::unbind();
    Texture::unbind();
}