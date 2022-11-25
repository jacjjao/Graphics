#include "../include/Camera2D.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"

#include <cmath>

Camera2D::Camera2D() noexcept :
m_view{Matrix3::identity()},
should_update{false},
m_position{Utility::getHalfWindowWidth(), Utility::getHalfWindowHeight()}, // center of the screen
dvector{},
dtheta{0.0F}
{
}

void Camera2D::use() noexcept
{
    if (should_update)
    {
        update();
    }
    ShaderProgram2D::instance().setMat3("view", m_view);
}

void Camera2D::move(const Vector2f vector) noexcept
{
    dvector += vector;
    m_position += vector;
    should_update = true;
}

void Camera2D::rotate(const float degree) noexcept
{
    dtheta += degree;
    should_update = true;
}

void Camera2D::scale(const Vector2f scale, const Vector2f vec_to_center) noexcept
{
    dscale         = scale;
    dvec_to_center = vec_to_center;
    should_update  = true;
}

void Camera2D::update() noexcept
{
    const auto [c, d]   = Utility::vectorToOpenGL(dvec_to_center);
    const auto [dx, dy] = Utility::vectorToOpenGL(dvector);
    const auto theta    = Utility::radians(dtheta);
    const auto ccos     = std::cos(theta);
    const auto ssin     = std::sin(theta);
    const auto h        = Utility::getWindowHeight();
    const auto w        = Utility::getWindowWidth();

    m_view[0][0] = dscale.x * ccos;
    m_view[0][1] = -h * dscale.y * ssin / w;
    m_view[0][2] = (c * dscale.x * w * ccos - h * d * dscale.y * ssin - c * w * ccos + h * d * ssin) / w + dx;

    m_view[1][0] = dscale.x * w * ssin / h;
    m_view[1][1] = dscale.y * ccos;
    m_view[1][2] = (c * dscale.x * w * ssin - c * w * ssin - h * d * ccos + h * d * dscale.y * ccos) / h + dy;

    should_update = false;
}