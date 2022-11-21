#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

class Transformable
{
public:
    Transformable() noexcept;
    virtual ~Transformable() = default;

    void translate(Vector2f vector) noexcept;
    void scale(Vector2f factor) noexcept;
    void rotate(float degree) noexcept;

    Matrix3& getTransformMatrix() noexcept;

    void setPosition(Vector2f position) noexcept;

    [[nodiscard]] Vector2f getPosition() const noexcept;

private:
    bool     should_update;
    float    dtheta;
    Vector2f dscale;

    Vector2f m_position;

    Matrix3 m_model;
};