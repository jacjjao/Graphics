#include "pch.hpp"
#include "../include/Rectangle2D.hpp"
#include "../include/ShaderProgram.hpp"

#include <glad/glad.h>

Rectangle2D::Rectangle2D(const Vector2f size) : 
Shape{4}, 
m_ebo{6}, 
m_size{size}
{
    create();
}

void Rectangle2D::draw()
{
    m_vao.drawIndices(
        m_ebo.size(), 
        PrimitiveType::Triangles, 
        getTransformMatrix(), 
        hasTexture() ? 0.0F : 1.0F, 
        m_texture
    );
}

void Rectangle2D::update()
{
    // update position
    const auto half_size   = m_size / 2.0F;

    const Vector2f top_right_pos{half_size.x, half_size.y};
    const Vector2f bottom_right_pos{half_size.x, -half_size.y};
    const Vector2f bottom_left_pos{-half_size.x, -half_size.y};
    const Vector2f top_left_pos{-half_size.x, half_size.y};

    m_vao[0].position = top_right_pos;
    m_vao[1].position = bottom_right_pos;
    m_vao[2].position = bottom_left_pos;
    m_vao[3].position = top_left_pos;
    // update color
    for (auto& vertex : m_vao)
    {
        vertex.color = m_color;
    }
    // update texture coordinate
    if (hasTexture())
    {
        m_vao[0].tex_coord = m_tex_rect.position + m_tex_rect.size;
        m_vao[1].tex_coord = {m_tex_rect.position.x + m_tex_rect.size.x, m_tex_rect.position.y};
        m_vao[2].tex_coord = m_tex_rect.position;
        m_vao[3].tex_coord = {m_tex_rect.position.x, m_tex_rect.position.y + m_tex_rect.size.y};
    }

    if (m_vao.isCreated())
    {
        m_vao.update();
    }
}

void Rectangle2D::setWidth(const float width)
{
    m_size.x = width;
}

void Rectangle2D::setHeight(const float height)
{
    m_size.y = height;
}

void Rectangle2D::create()
{
    m_ebo = { 0, 1, 3, 1, 2, 3 };
    m_ebo.update(); 
    m_vao.setElementBuffer(m_ebo);
}
