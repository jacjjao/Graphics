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

} // namespace Engine