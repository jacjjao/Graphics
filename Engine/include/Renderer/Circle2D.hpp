#pragma once

#include "Shape.hpp"

namespace eg
{

    class Circle2D : public Shape
    {
    public:
        explicit Circle2D(float radius, size_t point_count = 30);
        ~Circle2D() override = default;

        Circle2D(Circle2D&&) = default;
        Circle2D& operator=(Circle2D&&) = default;
        
        void draw() override;
        void update() override;

        void setRadius(float radius) { m_radius = radius; }

        [[nodiscard]]
        size_t getPointCount() const override { return m_vao.size() - 2; }
        [[nodiscard]]
        Vector2f getPoint(const size_t index) override;
        [[nodiscard]]
        void getAllTransformPoint(std::vector<Vector2f>& container, size_t from = 0) const override;
    private:
        float m_radius;
    };

} // namespace eg