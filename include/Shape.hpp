#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "Vertex.hpp"
#include <glm/ext/vector_float2.hpp>

class Shape : public Drawable, public Transformable
{
public:
    Shape();

    virtual void update() = 0;

    void setPosition(glm::vec2 position) noexcept;
    void setColor(Color color) noexcept;

    [[nodiscard]] glm::vec2 getPosition() const noexcept;
    [[nodiscard]] Color getColor() const noexcept;

    void translate(glm::vec2 vector) noexcept override;
    void scale(glm::vec2 factor) noexcept override;
    void rotate(float degree) noexcept override;

protected:
    glm::vec2 m_position;
    Color m_color;
};