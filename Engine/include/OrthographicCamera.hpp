#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

namespace Engine
{

    class OrthographicCamera
    {
    public:
        explicit OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0F, float zFar = 1.0F);

        void move(Vector3f vector);
        void moveTo(Vector3f position);
        void rotate(float degree);
        void scale(Vector2f scale);

        [[nodiscard]] Vector3f getPosition() const { return m_position; }
        [[nodiscard]] const Matrix4& getViewMatrix();
        [[nodiscard]] const Matrix4& getProjMatrix() const { return m_proj; }

        OrthographicCamera(const OrthographicCamera&) = delete;
        OrthographicCamera& operator=(const OrthographicCamera&) = delete;

    private:
        bool should_update;

        Matrix4 m_view;
        Matrix4 m_proj;

        Vector3f m_position;

        Vector2f m_scale;

        float m_degree;
    };

} // namespace Engine