#include "../include/Transformable.hpp"
#include "../include/Math.hpp"

#include <cmath>


Transformable::Transformable() noexcept :
should_update{false},
dtheta{0.0F},
dscale{1.0F, 1.0F},
m_position{0.0F, 0.0F, 0.0F},
m_model{Matrix4::makeIdentity()}
{
}

void Transformable::translate(const Vector3f vector) noexcept
{
    m_position += vector;
    should_update = true;
}

void Transformable::scale(const Vector2f factor) noexcept
{
    dscale        = factor;
    should_update = true;
}

void Transformable::rotate(const float degree) noexcept
{
    dtheta += degree;
    should_update = true;
}

const Matrix4& Transformable::getTransformMatrix() noexcept
{
    if (should_update)
    {
        const auto theta = radians(dtheta);
        const auto ssin  = std::sin(theta);
        const auto ccos  = std::cos(theta);

        m_model[0][0] = dscale.x * ccos;
        m_model[0][1] = -dscale.y * ssin;
        m_model[0][3] = m_position.x;

        m_model[1][0] = dscale.x * ssin;
        m_model[1][1] = dscale.y * ccos;
        m_model[1][3] = m_position.y;

        should_update = false;
    }

    return m_model;
}

void Transformable::setPosition(const Vector3f position) noexcept
{
    m_position    = position;
    should_update = true;
}

Vector3f Transformable::getPosition() const noexcept
{
    return m_position;
}