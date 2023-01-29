#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

class Camera
{
public:
    explicit Camera(float left, float right, float bottom, float top, float zNear = -1.0F, float zFar = 1.0F);

    void use();

    void move(Vector3f vector);
    void moveTo(Vector3f position);
    void rotate(float degree);
    void scale(Vector2f scale);

    [[nodiscard]] Vector3f getPosition() const;
    [[nodiscard]] const Matrix4& getViewMatrix();
    [[nodiscard]] const Matrix4& getProjMatrix() const;

    Camera(const Camera&)            = delete;
    Camera& operator=(const Camera&) = delete;

private:
    void update();

    bool should_update;

    Matrix4 m_view;
    Matrix4 m_proj;

    Vector3f m_position;

    Vector2f dscale;

    float dtheta;
};