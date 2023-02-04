#pragma once

#include "Transformable.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "include/Core/Rect.hpp"

namespace Engine
{

    class Shape : public Transformable
    {
    public:
        explicit Shape(size_t size);
        virtual ~Shape() = default;

        Shape(Shape&&) = default;
        Shape& operator=(Shape&&) = default;

        virtual void update() = 0;
        virtual void draw() = 0;

        void applyTexture(Texture* texture) { m_texture = texture; }

        [[nodiscard]] 
        bool hasTexture() const { return (m_texture != nullptr); }

        void setColor(Color color) { m_color = color; }

        [[nodiscard]] 
        Color getColor() const { return m_color; }

        void setTextureRect(Rect rect) { m_tex_rect = rect; }

        [[nodiscard]] 
        Rect getTextureRect() const { return m_tex_rect; }

    protected:
        VertexArray m_vao;

        Color m_color;

        Texture* m_texture;

        Rect m_tex_rect;
    };

} // namespace Engine