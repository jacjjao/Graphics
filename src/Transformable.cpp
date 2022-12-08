#include "../include/Transformable.hpp"

#include <cmath>

#include "../include/Utility.hpp"

Transformable::Transformable() noexcept :
m_model{Matrix4::makeIdentity()},
m_position{},
should_update{false},
dtheta{},
dscale{1.0F, 1.0F}
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
        Matrix4::toIdentity(m_model);

        const auto half_win_width    = Utility::getHalfWindowWidth();
        const auto half_win_height   = Utility::getHalfWindowHeight();
        const auto center_to_pos_vec = Vector3f{m_position.x - half_win_width, m_position.y - half_win_height, 0.0F};
        const auto [dx, dy, _]       = Utility::vectorToOpenGL(center_to_pos_vec);
        const auto h                 = half_win_height;
        const auto w                 = half_win_width;
        const auto theta             = Utility::radians(dtheta);
        const auto ssin              = std::sin(theta);
        const auto ccos              = std::cos(theta);

        m_model[0][0] = dscale.x * ccos;
        m_model[0][1] = -dscale.y * h * ssin / w;
        m_model[0][3] = dx;

        m_model[1][0] = dscale.x * w * ssin / h;
        m_model[1][1] = dscale.y * ccos;
        m_model[1][3] = dy;

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