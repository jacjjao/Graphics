#pragma once

#include "Drawable.hpp"
#include "Transformable.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"

class Shape : public Drawable, public Transformable
{
public:
    Shape() noexcept = default;

    virtual void update() = 0;

    void setColor(Color color) noexcept;
    void applyTexture(Texture* texture) noexcept;

    [[nodiscard]] Color getColor() const noexcept;

protected:
    Texture* m_texture = nullptr;
    Color    m_color;
};