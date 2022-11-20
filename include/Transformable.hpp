#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

class Transformable
{
public:
    Transformable() noexcept;
    virtual ~Transformable() = default;

    void translate(Vector2<float> vector) noexcept;
    void scale(Vector2<float> factor) noexcept;
    void rotate(float degree) noexcept;

    Matrix4<float>& getTransformMatrix() noexcept;

    void setPosition(Vector2<float> position) noexcept;

    [[nodiscard]] Vector2<float> getPosition() const noexcept;

private:
    bool           should_update;
    float          dtheta;
    Vector2<float> dvec;
    Vector2<float> dscale;

    Vector2<float> m_position;

    Matrix4<float> m_model;
};