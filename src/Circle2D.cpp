#include "../include/ShaderProgram.hpp"
#include "../include/glCheck.hpp"
#include "../include/Circle2D.hpp"

#include <glad/glad.h>

#include <cmath>
#include <numbers>

Circle2D::Circle2D(const float radius, const size_t point_count) noexcept : Shape{point_count}, m_radius{radius}
{
}

void Circle2D::draw() noexcept
{
    if (!m_vao.isAvailable())
    {
        create();
    }

    setupDraw();
    glCheck(glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(m_vao.size())));
    VertexArray::unbind();
}

void Circle2D::update() noexcept
{
    constexpr auto pi = std::numbers::pi_v<float>;

    const auto f_point_count = static_cast<float>(m_vao.size() - 1);
    const auto slice    = 2.0F * pi / f_point_count;
    const auto center = getPosition();

    const auto half_tex_width  = m_tex_rect.size.x / 2.0F;
    const auto half_tex_height = m_tex_rect.size.y / 2.0F;

    m_vao[0].position  = center;
    m_vao[0].color     = m_color;
    m_vao[0].tex_coord = (hasTexture()) ? m_tex_rect.position : Vector2f{0.0F, 0.0F};
    for (size_t i = 1; i < m_vao.size() - 1; i++)
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
        if (hasTexture())
        {
            Vector2f tex_coord{};

            tex_coord.x = m_tex_rect.position.x + half_tex_width * ccos;
            tex_coord.y = m_tex_rect.position.y + half_tex_height * ssin;

            m_vao[i].tex_coord = tex_coord;
        }
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
    auto& program = ShaderProgram2D::instance();

    program.setFloat("color_alpha", hasTexture() ? 0.0F : 1.0F);
    program.setMat4("model", getTransformMatrix());

    VertexArray::bind(&m_vao);
}