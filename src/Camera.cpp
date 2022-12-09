#include "../include/Camera.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Window.hpp"

#include <cmath>

Camera::Camera() noexcept :
m_view{Matrix4::makeIdentity()},
should_update{true},
dtheta{0.0F},
dscale{1.0F, 1.0F},
m_position{Window::getHalfWindowWidth(), Window::getHalfWindowHeight(), 0.0F}
{
}

void Camera::use() noexcept
{
    ShaderProgram2D::instance().setMat4("view", getViewMatrix());
}

void Camera::move(const Vector3f vector) noexcept
{
    const auto theta = Window::radians(dtheta);
    const auto ccos  = std::cos(theta);
    const auto ssin  = std::sin(theta);

    auto vec = vector;

    vec.x = (vector.x * ccos - vector.y * ssin) / dscale.x;
    vec.y = (vector.x * ssin + vector.y * ccos) / dscale.y;

    m_position += vec;
    should_update = true;
}

void Camera::moveTo(const Vector3f position) noexcept
{
    m_position    = position;
    should_update = true;
}

void Camera::rotate(const float degree) noexcept
{
    dtheta += degree;
    should_update = true;
}

void Camera::scale(const Vector2f scale) noexcept
{
    dscale        = scale;
    should_update = true;
}

void Camera::update() noexcept
{
    const auto [x, y, _] = -(Window::pointToOpenGL(m_position));
    const auto theta     = Window::radians(dtheta);
    const auto ccos      = std::cos(theta);
    const auto ssin      = std::sin(theta);
    const auto h         = Window::getWindowHeight();
    const auto w         = Window::getWindowWidth();
    const auto [a, b]    = dscale;

    m_view[0][0] = a * ccos;
    m_view[0][1] = -b * h * ssin / w;
    m_view[0][3] = (a * x * w * ccos - a * h * y * ssin) / w;

    m_view[1][0] = a * w * ssin / h;
    m_view[1][1] = b * ccos;
    m_view[1][3] = (b * h * y * ccos + b * w * x * ssin) / h;

    should_update = false;
}

Vector3f Camera::getPosition() const noexcept
{
    return m_position;
}

Matrix4& Camera::getViewMatrix() noexcept
{
    if (should_update)
    {
        update();
    }
    return m_view;
}