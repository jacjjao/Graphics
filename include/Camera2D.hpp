#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
class Camera2D
{
public:
    Camera2D() noexcept;

    void use() noexcept;

    void move(Vector2f vector) noexcept;
    void moveTo(Vector2f position) noexcept;
    void rotate(float degree) noexcept;
    void scale(Vector2f scale) noexcept;

    [[nodiscard]] Vector2f getPosition() const noexcept;

private:
    void update() noexcept;

    bool should_update;

    Matrix3 m_view;

    Vector2f m_position;
    Vector2f dscale;
    float    dtheta;
};