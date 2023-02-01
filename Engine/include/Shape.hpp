#pragma once

#include "Transformable.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Rect.hpp"

namespace Engine
{

    class Shape : public Transformable
    {
    public:
        explicit Shape(size_t size);
        virtual ~Shape() = default;

        Shape(const Shape&) = delete;
        Shape& operator=(const Shape&) = delete;

        virtual void update() = 0;
        virtual void draw() = 0;

        void applyTexture(Texture* texture);

        [[nodiscard]] bool hasTexture() const;

        void setColor(Color color);

        [[nodiscard]] Color getColor() const;

        void setTextureRect(Rect rect);

        [[nodiscard]] Rect getTextureRect() const;

    protected:
        VertexArray m_vao;

        Color m_color;

        Texture* m_texture;

        Rect m_tex_rect;
    };

} // namespace Engine