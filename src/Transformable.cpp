#include "../include/Transformable.hpp"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>

#include "../include/Utility.hpp"

Transformable::Transformable() noexcept :
m_model{1.0F},
m_position{},
should_update{false},
dvec{},
dtheta{},
dscale{1.0F, 1.0F}
{
}

void Transformable::translate(const glm::vec2 vector) noexcept
{
    dvec += vector;
    should_update = true;
}

void Transformable::scale(const glm::vec2 factor) noexcept
{
    dscale        = factor;
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

        glm::vec2 vec_to_center{Utility::getHalfWindowWidth() - m_position.x,
                                Utility::getHalfWindowHeight() - m_position.y};
        vec_to_center      = Utility::vectorToOpenGL(vec_to_center);
        const auto h       = Utility::getHalfWindowHeight();
        const auto w       = Utility::getHalfWindowWidth();
        const auto theta   = glm::radians(dtheta);
        const auto ssin    = std::sin(theta);
        const auto ccos    = std::cos(theta);
        const auto dvector = Utility::vectorToOpenGL(dvec);

        m_model[0][0] = dscale.x * ccos;
        m_model[1][0] = -dscale.y * h * ssin / w;
        m_model[3][0] = ((dscale.x * vec_to_center.x * w * ccos - dscale.y * vec_to_center.y * h * ssin) / w) -
                        vec_to_center.x + dvector.x;

        m_model[0][1] = dscale.x * w * ssin / h;
        m_model[1][1] = dscale.y * ccos;
        m_model[3][1] = ((dscale.y * vec_to_center.y * h * ccos + dscale.x * vec_to_center.x * w * ssin) / h) -
                        vec_to_center.y + dvector.y;

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