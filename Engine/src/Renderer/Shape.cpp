#include "pch.hpp"
#include "include/Renderer/Shape.hpp"
#include "include/Core/Math.hpp"

namespace eg
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
        const auto p = m_vao[index].position;
        /*
        Matrix<float, 4, 1> mat = {p.x, p.y, 0, 1};
        const auto result = getTransformMatrix() * mat;
        return Vector2f{ result[0][0], result[1][0] };*/
        
        const auto scale  = getScale();
        const auto degree = getRotateDegree();
        const auto [x, y] = getPosition();
        const auto origin = getOrigin();
        const auto cosine = std::cos(radians(degree));
        const auto sine   = std::sin(radians(degree));

        eg::Vector2f tp{};
        tp.x = 
            scale.x * p.x * cosine + 
            scale.y * origin.y * sine - 
            scale.y * p.y * sine - 
            scale.x * origin.x * cosine + x;
        tp.y =
            scale.x * p.x * sine +
            scale.y * p.y * cosine -
            scale.x * origin.x * sine -
            scale.y * origin.y * cosine + y;

        return tp;
    }

    void Shape::getAllTransformPoint(std::vector<Vector2f>& container, const size_t from) const
    {
        const auto scale = getScale();
        const auto degree = getRotateDegree();
        const auto [x, y] = getPosition();
        const auto origin = getOrigin();
        const auto cosine = std::cos(radians(degree));
        const auto sine = std::sin(radians(degree));

        container.resize(m_vao.size());
        for (size_t i = from; i < m_vao.size(); i++)
        {
            const auto p = m_vao[i].position;
            Vector2f result{};
            result.x =
                scale.x * p.x * cosine +
                scale.y * origin.y * sine -
                scale.y * p.y * sine -
                scale.x * origin.x * cosine + x;
            result.y =
                scale.x * p.x * sine +
                scale.y * p.y * cosine -
                scale.x * origin.x * sine -
                scale.y * origin.y * cosine + y;
            container[i] = result;
        }
    }

} // namespace eg