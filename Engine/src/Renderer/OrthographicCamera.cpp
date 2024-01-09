#include "pch.hpp"
#include "include/Renderer/OrthographicCamera.hpp"
#include "include/Core/Math.hpp"

namespace eg
{

    OrthographicCamera::OrthographicCamera(const float width, const float height) :
        m_view{ Matrix4::makeIdentity() },
        should_update{ true },
        m_degree{ 0.0F },
        m_scale{ 1.0F },
        m_position{ 0.0F }
    {
        m_proj = ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    }

    void OrthographicCamera::move(const Vector3f vector)
    {
        const float theta  = radians(-m_degree);
        const float cosine = std::cos(theta);
        const float sine   = std::sin(theta);

        eg::Vector3f vec = vector;

        vec.x = (vector.x * cosine - vector.y * sine) / m_scale.x;
        vec.y = (vector.x * sine + vector.y * cosine) / m_scale.y;

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
            const auto [a, b] = m_scale;

            const float theta = radians(m_degree);
            const float cosine = std::cos(theta);
            const float sine = std::sin(theta);

            m_view[0][0] = a * cosine;
            m_view[0][1] = -b * sine;
            m_view[0][3] = -(a * x * cosine - a * y * sine);

            m_view[1][0] = a * sine;
            m_view[1][1] = b * cosine;
            m_view[1][3] = -(b * y * cosine + b * x * sine);

            should_update = false;
        }
        return m_view;
    }

} // namespace eg