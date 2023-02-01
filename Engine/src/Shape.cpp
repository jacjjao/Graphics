#include "pch.hpp"
#include "../include/Shape.hpp"

namespace Engine
{

    Shape::Shape(const size_t size) :
        m_vao{ size },
        m_color{ Color::White },
        m_texture{ nullptr },
        m_tex_rect{
            Vector2f{0.0F, 0.0F},
            Vector2f{1.0F, 1.0F}
        }
    {
    }

    void Shape::applyTexture(Texture* texture)
    {
        m_texture = texture;
    }

    bool Shape::hasTexture() const
    {
        return (m_texture != nullptr);
    }

    void Shape::setColor(const Color color)
    {
        m_color = color;
    }

    Color Shape::getColor() const
    {
        return m_color;
    }

    void Shape::setTextureRect(const Rect rect)
    {
        m_tex_rect = rect;
    }

    Rect Shape::getTextureRect() const
    {
        return m_tex_rect;
    }

} // namespace Engine