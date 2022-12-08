#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
class Camera
{
public:
    Camera() noexcept;

    void use() noexcept;

    void move(Vector3f vector) noexcept;
    void moveTo(Vector3f position) noexcept;
    void rotate(float degree) noexcept;
    void scale(Vector2f scale) noexcept;

    [[nodiscard]] Vector3f getPosition() const noexcept;
    [[nodiscard]] Matrix4& getViewMatrix() noexcept;

    Camera(const Camera&)            = delete;
    Camera& operator=(const Camera&) = delete;

private:
    void update() noexcept;

    bool should_update;

    Matrix4 m_view;

    Vector3f m_position;

    Vector2f dscale;

    float dtheta;
};