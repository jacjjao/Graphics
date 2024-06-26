#pragma once

#include "Transformable.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "include/Core/Rect.hpp"

namespace eg
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

        virtual size_t getPointCount() const = 0;
        virtual Vector2f getPoint(size_t index);
        virtual void getAllTransformPoint(std::vector<Vector2f>& container, size_t from = 0) const;

        void applyTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

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

        std::shared_ptr<Texture> m_texture;

        Rect m_tex_rect;
    };

} // namespace eg