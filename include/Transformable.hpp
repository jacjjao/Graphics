#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

class Transformable
{
public:
    Transformable() noexcept;
    virtual ~Transformable() noexcept = default;

    void translate(Vector3f vector) noexcept;
    void scale(Vector2f factor) noexcept;
    void rotate(float degree) noexcept;

    const Matrix4& getTransformMatrix() noexcept;

    void setPosition(Vector3f position) noexcept;

    [[nodiscard]] Vector3f getPosition() const noexcept;

private:
    bool     should_update;
    float    m_theta;
    Vector2f m_scale;

    Vector3f m_position;

    Matrix4 m_model;
};