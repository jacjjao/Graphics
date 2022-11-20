#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>

class Transformable
{
public:
    Transformable() noexcept;
    virtual ~Transformable() = default;

    void translate(glm::vec2 vector) noexcept;
    void scale(glm::vec2 factor) noexcept;
    void rotate(float degree) noexcept;

    glm::mat4 getTransformMatrix() noexcept;

    void setPosition(glm::vec2 position) noexcept;

    [[nodiscard]] glm::vec2 getPosition() const noexcept;

private:
    bool      should_update;
    float     dtheta;
    glm::vec2 dvec;
    glm::vec2 dscale;

    glm::vec2 m_position;

    glm::mat4 m_model;
};