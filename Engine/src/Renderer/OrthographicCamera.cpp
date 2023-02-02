#include "pch.hpp"
#include "include/Renderer/OrthographicCamera.hpp"
#include "include/Core/Math.hpp"

namespace Engine
{

    OrthographicCamera::OrthographicCamera(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float zNear,
        const float zFar
    ) :
        m_view{ Matrix4::makeIdentity() },
        should_update{ true },
        m_degree{ 0.0F },
        m_scale{ 1.0F },
        m_position{ 0.0F }
    {
        m_proj = ortho(left, right, bottom, top, zNear, zFar);
    }

    void OrthographicCamera::move(const Vector3f vector)
    {
        const auto theta = radians(-m_degree);
        const auto ccos = std::cos(theta);
        const auto ssin = std::sin(theta);

        auto vec = vector;

        vec.x = (vector.x * ccos - vector.y * ssin) / m_scale.x;
        vec.y = (vector.x * ssin + vector.y * ccos) / m_scale.y;

        m_position += vec;
        should_update = true;
    }

    void OrthographicCamera::moveTo(const Vector3f position)
    {
        m_position = position;
        should_update = true;
    }

    void OrthographicCamera::rotate(const float degree)
    {
        m_degree += degree;
        should_update = true;
    }

    void OrthographicCamera::scale(const Vector2f scale)
    {
        m_scale = scale;
        should_update = true;
    }

    const Matrix4& OrthographicCamera::getViewMatrix()
    {
        if (should_update)
        {
            const auto [x, y, _] = m_position;
            const auto theta = radians(m_degree);
            const auto ccos = std::cos(theta);
            const auto ssin = std::sin(theta);
            const auto [a, b] = m_scale;

            m_view[0][0] = a * ccos;
            m_view[0][1] = -b * ssin;
            m_view[0][3] = -(a * x * ccos - a * y * ssin);

            m_view[1][0] = a * ssin;
            m_view[1][1] = b * ccos;
            m_view[1][3] = -(b * y * ccos + b * x * ssin);

            should_update = false;
        }
        return m_view;
    }

} // namespace Engine