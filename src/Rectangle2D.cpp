#include "../include/Rectangle2D.hpp"

#include <glad/glad.h>

#include "../include/ShaderProgram.hpp"
#include "../include/Window.hpp"
#include "../include/glCheck.hpp"

Rectangle2D::Rectangle2D(const Vector2f size) noexcept : Shape{4}, m_ebo{6}, m_size{size}
{
}

void Rectangle2D::draw() noexcept
{
    if (!m_vao.isAvailable())
    {
        create();
    }

    setupDraw();
    glCheck(glDrawElements(GL_TRIANGLES, m_ebo.size(), GL_UNSIGNED_INT, 0));
    cleanUpDraw();
}

void Rectangle2D::update() noexcept
{
    // update position
    const auto half_width  = static_cast<float>(m_size.x) / 2.0F;
    const auto half_height = static_cast<float>(m_size.y) / 2.0F;

    const Vector2f center{Window::getHalfWindowWidth(), Window::getHalfWindowHeight()};
    const Vector2f top_right_pos{center.x + half_width, center.y - half_height};
    const Vector2f bottom_right_pos{center.x + half_width, center.y + half_height};
    const Vector2f bottom_left_pos{center.x - half_width, center.y + half_height};
    const Vector2f top_left_pos{center.x - half_width, center.y - half_height};

    m_vao[0].position = top_right_pos;
    m_vao[1].position = bottom_right_pos;
    m_vao[2].position = bottom_left_pos;
    m_vao[3].position = top_left_pos;
    // update color
    const auto color = getColor();
    for (auto& vertex : m_vao)
    {
        vertex.color = color;
    }
    // update texture coordinate
    if (hasTexture())
    {
        const auto* texture    = m_vao.getTexture();
        const auto  tex_width  = texture->getWidth();
        const auto  tex_height = texture->getHeight();

        m_vao[0].tex_coord = Vector2f{tex_width, 0.0F};
        m_vao[1].tex_coord = Vector2f{tex_width, tex_height};
        m_vao[2].tex_coord = Vector2f{0.0F, tex_height};
        m_vao[3].tex_coord = Vector2f{0.0F, 0.0F};
    }

    if (m_vao.isAvailable())
    {
        m_vao.update();
    }
}

void Rectangle2D::setWidth(const float width) noexcept
{
    m_size.x = width;
}

void Rectangle2D::setHeight(const float height) noexcept
{
    m_size.y = height;
}

void Rectangle2D::create() noexcept
{
    update();

    m_vao.create();
    VertexArray::bind(&m_vao);

    m_ebo = {0, 1, 3, 1, 2, 3};
    m_ebo.create();
    ElementBuffer::bind(&m_ebo);

    VertexArray::unbind();
    ElementBuffer::unbind();
}

void Rectangle2D::setupDraw() noexcept
{
    if (hasTexture())
    {
        glCheck(glActiveTexture(GL_TEXTURE0));
        Texture::bind(m_vao.getTexture());
    }

    auto& program = ShaderProgram2D::instance();

    program.setFloat("color_alpha", hasTexture() ? 0.0F : 1.0F);
    program.setMat4("model", getTransformMatrix());

    VertexArray::bind(&m_vao);
}

void Rectangle2D::cleanUpDraw() noexcept
{
    VertexArray::unbind();
    Texture::unbind();
}