#pragma once

#include "include/Core/Matrix.hpp"
#include "include/Core/Vector.hpp"

namespace eg
{

    class Transformable
    {
    public:
        Transformable();
        virtual ~Transformable() = default;

        Transformable(Transformable&&) = default;
        Transformable& operator=(Transformable&&) = default;

        void translate(Vector2f vector);
        void scale(Vector2f factor);
        void rotate(float degree);
        void setRotation(float degree);

        const Matrix4& getTransformMatrix();

        void setPosition(Vector2f position);

        [[nodiscard]] 
        Vector2f getPosition() const { return m_position; }
        [[nodiscard]]
        Vector2f getScale() const { return m_scale; }
        [[nodiscard]]
        Vector2f getOrigin() const { return m_origin; }
        [[nodiscard]]
        float getRotateDegree() const { return m_theta; }


    protected:
        void setOrigin(const Vector2f origin) { m_origin = origin; }

    private:
        bool should_update = false;
        float m_theta = 0.0F;

        Vector2f m_scale;

        Vector2f m_position, m_origin;

        Matrix4 m_model;
    };

} // namespace eg