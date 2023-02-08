#include "pch.hpp"
#include "include/Renderer/Shape.hpp"

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

    Vector2f Shape::getPoint(const size_t index)
    {
        const auto pos = m_vao[index].position;
        Matrix<float, 4, 1> mat = { pos.x, pos.y, 0, 1 };
        const auto result = getTransformMatrix() * mat;
        return Vector2f{ result[0][0], result[1][0] };
    }

} // namespace Engine