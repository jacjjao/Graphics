#pragma once

#include "ElementBuffer.hpp"
#include "Shape.hpp"

namespace Engine
{

    class Rectangle2D : public Shape
    {
    public:
        explicit Rectangle2D(float width, float height);
        ~Rectangle2D() override = default;

        Rectangle2D(const Rectangle2D&) = delete;
        Rectangle2D& operator=(const Rectangle2D&) = delete;

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

    private:
        float m_width, m_height;

        ElementBuffer m_ebo;
    };

} // namespace Engine