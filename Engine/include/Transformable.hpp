#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

namespace Engine
{

    class Transformable
    {
    public:
        Transformable();
        virtual ~Transformable() = default;

        void translate(Vector3f vector);
        void scale(Vector2f factor);
        void rotate(float degree);

        const Matrix4& getTransformMatrix();

        void setPosition(Vector3f position);

        [[nodiscard]] Vector3f getPosition() const;

    private:
        bool should_update = false;
        float m_theta = 0.0F;

        Vector2f m_scale;

        Vector3f m_position;

        Matrix4 m_model;
    };

} // namespace Engine