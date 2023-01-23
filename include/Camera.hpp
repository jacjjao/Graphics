#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
class Camera
{
public:
    explicit Camera(float left, float right, float bottom, float top, float zNear = -1.0F, float zFar = 1.0F) noexcept;

    void use() noexcept;

    void move(Vector3f vector) noexcept;
    void moveTo(Vector3f position) noexcept;
    void rotate(float degree) noexcept;
    void scale(Vector2f scale) noexcept;

    [[nodiscard]] Vector3f getPosition() const noexcept;
    [[nodiscard]] const Matrix4& getViewMatrix() noexcept;
    [[nodiscard]] const Matrix4& getProjMatrix() const noexcept;

    Camera(const Camera&)            = delete;
    Camera& operator=(const Camera&) = delete;

private:
    void update() noexcept;

    bool should_update;

    Matrix4 m_view;
    Matrix4 m_proj;

    Vector3f m_position;

    Vector2f dscale;

    float dtheta;
};