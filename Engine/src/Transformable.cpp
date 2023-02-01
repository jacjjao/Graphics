#include "pch.hpp"
#include "../include/Transformable.hpp"
#include "include/Utility/Math.hpp"


namespace Engine
{

    Transformable::Transformable() :
        m_scale{ 1.0F },
        m_position{ 0.0F },
        m_model{ Matrix4::makeIdentity() }
    {
    }

    void Transformable::translate(const Vector3f vector)
    {
        m_position += vector;
        should_update = true;
    }

    void Transformable::scale(const Vector2f factor)
    {
        m_scale = factor;
        should_update = true;
    }

    void Transformable::rotate(const float degree)
    {
        m_theta += degree;
        should_update = true;
    }

    const Matrix4& Transformable::getTransformMatrix()
    {
        if (should_update)
        {
            const auto theta = radians(m_theta);
            const auto sine = std::sin(theta);
            const auto cosine = std::cos(theta);
            const auto [x, y, _] = m_position;

            m_model[0][0] = m_scale.x * cosine;
            m_model[0][1] = -m_scale.y * sine;
            m_model[0][3] = x;

            m_model[1][0] = m_scale.x * sine;
            m_model[1][1] = m_scale.y * cosine;
            m_model[1][3] = y;

            should_update = false;
        }

        return m_model;
    }

    void Transformable::setPosition(const Vector3f position)
    {
        m_position = position;
        should_update = true;
    }

    Vector3f Transformable::getPosition() const
    {
        return m_position;
    }

} // namespace Engine