#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"

namespace eg
{

    class Rectangle2D : public Shape
    {
    public:
        Rectangle2D();
        explicit Rectangle2D(float width, float height);
        ~Rectangle2D() override = default;

        Rectangle2D(Rectangle2D&&) noexcept = default;
        Rectangle2D& operator=(Rectangle2D&&) noexcept = default;

        void draw() override;
        void update() override;

        void setWidth(float width) { m_width = width; }
        void setHeight(float height) { m_height = height; }

        [[nodiscard]]
        float getWidth() const { return m_width; }
        [[nodiscard]]
        float getHeight() const { return m_height; }

        [[nodiscard]]
        size_t getPointCount() const override { return 4; }
    private:
        float m_width, m_height;

        ElementBuffer m_ebo;
    };

} // namespace eg