#include "../include/pch.hpp"
#include "../include/Circle2D.hpp"

#include <glad/glad.h>

#include <numbers>

Circle2D::Circle2D(const Vector2f radius, const size_t point_count) noexcept :
Shape{point_count + 2},
m_radius{radius}
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

    const auto f_point_count = static_cast<float>(m_vao.size() - 2);
    const auto slice    = 2.0F * pi / f_point_count;
    const auto tex_center    = m_tex_rect.position + m_tex_rect.size / 2.0F;
    const auto half_tex_size = m_tex_rect.size / 2.0F;

    m_vao[0].position  = Vector3f{0.0F, 0.0F, 0.0F};
    m_vao[0].color     = m_color;
    m_vao[0].tex_coord = tex_center;
    for (size_t i = 1; i < m_vao.size() - 1; i++)
    {
        const auto theta = slice * static_cast<float>(i - 1);
        const auto ssin  = std::sin(theta);
        const auto ccos  = std::cos(theta);

        constexpr auto aspect = 2000.0F / 1200.0F;

        Vector2f position{};
        position.x = m_radius.x * ccos;
        position.y = m_radius.y * ssin;

        m_vao[i].position = position;
        m_vao[i].color    = m_color;
        if (hasTexture())
        {
            Vector2f tex_coord{};

            tex_coord.x = tex_center.x + half_tex_size.x * ccos;
            tex_coord.y = tex_center.y + half_tex_size.y * ssin;

            m_vao[i].tex_coord = tex_coord;
        }
    }
    m_vao.back() = m_vao[1];

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Circle2D::setRadius(const Vector2f radius) noexcept
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