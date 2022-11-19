#include "../include/Transformable.hpp"
#include "../include/Utility.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <cmath>
#include <glm/trigonometric.hpp>

Transformable::Transformable()
    : m_model{1.0F}, m_position{}, should_update{false}, dx{}, dy{}, dtheta{}, dscale{1.0F, 1.0F}
{
}

void Transformable::translate(const glm::vec2 vector) noexcept
{
    dx += vector.x;
    dy += vector.y;
    should_update = true;
}

void Transformable::scale(const glm::vec2 factor) noexcept
{
    dscale *= factor;
    should_update = true;
}

void Transformable::rotate(const float degree) noexcept
{
    dtheta += degree;
    should_update = true;
}

glm::mat4 Transformable::getTransformMatrix() noexcept
{
    if (should_update)
    {
        m_model = glm::mat4{1.0F};

        const glm::vec2 vec_to_center{Utility::getHalfWindowWidth() - m_position.x,
                                      Utility::getHalfWindowHeight() - m_position.y};
        const auto vec_to_center_gl = Utility::vectorToOpenGL(vec_to_center);
        const auto h = Utility::getHalfWindowHeight();
        const auto w = Utility::getHalfWindowWidth();
        const float theta = glm::radians(dtheta);
        const float ssin = std::sin(theta);
        const float ccos = std::cos(theta);
        const auto dvector = Utility::vectorToOpenGL({dx, dy});

        m_model[0][0] = dscale.x * ccos;
        m_model[1][0] = -dscale.y * h * ssin / w;
        m_model[3][0] = ((dscale.x * vec_to_center_gl.x * w * ccos - dscale.y * vec_to_center_gl.y * h * ssin) / w) -
                        vec_to_center_gl.x + dvector.x;

        m_model[0][1] = dscale.x * w * ssin / h;
        m_model[1][1] = dscale.y * ccos;
        m_model[3][1] = ((dscale.y * vec_to_center_gl.y * h * ccos + dscale.x * vec_to_center_gl.x * w * ssin) / h) -
                        vec_to_center_gl.y + dvector.y;

        should_update = false;
    }

    return m_model;
}

void Transformable::setPosition(const glm::vec2 position) noexcept
{
    m_position = position;
}

glm::vec2 Transformable::getPosition() const noexcept
{
    return m_position;
}