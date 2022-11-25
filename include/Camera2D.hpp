#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
class Camera2D
{
public:
    Camera2D() noexcept;

    void use() noexcept;

    void move(Vector2f vector) noexcept;
    void rotate(float degree) noexcept;
    void scale(Vector2f scale, Vector2f vec_to_center) noexcept;

private:
    void update() noexcept;

    bool should_update;

    Matrix3 m_view;

    Vector2f m_position;

    Vector2f dvector;
    Vector2f dscale;
    Vector2f dvec_to_center;
    float    dtheta;
};