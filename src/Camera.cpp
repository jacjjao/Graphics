#include "../include/pch.hpp"
#include "../include/Camera.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Math.hpp"

Camera::Camera(
    const float left, 
    const float right, 
    const float bottom, 
    const float top, 
    const float zNear, 
    const float zFar
) :
m_view{Matrix4::makeIdentity()},
should_update{true},
dtheta{0.0F},
dscale{1.0F},
m_position{0.0F}
{
    m_proj = ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::use()
{
    DefaultShaderProgram::instance().setMat4("view", getViewMatrix());
    DefaultShaderProgram::instance().setMat4("proj", getProjMatrix());
}

void Camera::move(const Vector3f vector)
{ 
    const auto theta = radians(-dtheta);
    const auto ccos  = std::cos(theta);
    const auto ssin  = std::sin(theta);

    auto vec = vector;

    vec.x = (vector.x * ccos - vector.y * ssin) / dscale.x;
    vec.y = (vector.x * ssin + vector.y * ccos) / dscale.y;

    m_position += vec;
    should_update = true;
}

void Camera::moveTo(const Vector3f position)
{
    m_position    = position;
    should_update = true;
}

void Camera::rotate(const float degree)
{
    dtheta += degree;
    should_update = true;
}

void Camera::scale(const Vector2f scale)
{
    dscale        = scale;
    should_update = true;
}

void Camera::update()
{
    const auto [x, y, _] = m_position;
    const auto theta     = radians(dtheta);
    const auto ccos      = std::cos(theta);
    const auto ssin      = std::sin(theta);
    const auto [a, b]    = dscale;

    m_view[0][0] = a * ccos;
    m_view[0][1] = -b * ssin;
    m_view[0][3] = -(a * x * ccos - a * y * ssin);

    m_view[1][0] = a * ssin;
    m_view[1][1] = b * ccos;
    m_view[1][3] = -(b * y * ccos + b * x * ssin);

    should_update = false;
}

Vector3f Camera::getPosition() const
{
    return m_position;
}

const Matrix4& Camera::getViewMatrix()
{
    if (should_update)
    {
        update();
    }
    return m_view;
}

const Matrix4& Camera::getProjMatrix() const
{
    return m_proj;
}
