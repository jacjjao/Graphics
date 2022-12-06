#include "../include/Camera2D.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"

#include <cmath>

Camera2D::Camera2D() noexcept :
m_view{Matrix3::identity()},
should_update{true},
dtheta{0.0F},
dscale{1.0F, 1.0F},
m_position{Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()}
{
}

void Camera2D::use() noexcept
{
    ShaderProgram2D::instance().setMat3("view", getViewMatrix());
}

void Camera2D::move(const Vector2f vector) noexcept
{
    const auto theta = Utility::radians(dtheta);
    const auto ccos  = std::cos(theta);
    const auto ssin  = std::sin(theta);

    auto vec = vector;

    vec.x = (vector.x * ccos - vector.y * ssin) / dscale.x;
    vec.y = (vector.x * ssin + vector.y * ccos) / dscale.y;

    m_position += vec;
    should_update = true;
}

void Camera2D::moveTo(const Vector2f position) noexcept
{
    m_position    = position;
    should_update = true;
}

void Camera2D::rotate(const float degree) noexcept
{
    dtheta += degree;
    should_update = true;
}

void Camera2D::scale(const Vector2f scale) noexcept
{
    dscale        = scale;
    should_update = true;
}

void Camera2D::update() noexcept
{
    const auto [x, y] = -(Utility::pointToOpenGL(m_position));
    const auto theta  = Utility::radians(dtheta);
    const auto ccos   = std::cos(theta);
    const auto ssin   = std::sin(theta);
    const auto h      = Utility::getWindowHeight();
    const auto w      = Utility::getWindowWidth();
    const auto [a, b] = dscale;

    m_view[0][0] = a * ccos;
    m_view[0][1] = -b * h * ssin / w;
    m_view[0][2] = (a * x * w * ccos - a * h * y * ssin) / w;

    m_view[1][0] = a * w * ssin / h;
    m_view[1][1] = b * ccos;
    m_view[1][2] = (b * h * y * ccos + b * w * x * ssin) / h;

    should_update = false;
}

Vector2f Camera2D::getPosition() const noexcept
{
    return m_position;
}

Matrix3& Camera2D::getViewMatrix() noexcept
{
    if (should_update)
    {
        update();
    }
    return m_view;
}