#pragma once

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"
#include "include/Core/PrimitiveType.hpp"
#include "include/Core/Matrix.hpp"
#include "Texture.hpp"

#include <vector>

namespace eg
{

    class VertexArray
    {
    private:
        using element_type   = Vertex;
        using container      = std::vector<Vertex>;
        using value_type     = container::value_type;
        using iterator       = container::iterator;
        using const_iterator = container::const_iterator;

    public:
        explicit VertexArray(size_t size, VertexBuffer::Usage usage = VertexBuffer::Usage::StaticDraw);
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) const = delete;

        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void update();
        void draw(PrimitiveType primitive_type = PrimitiveType::Triangles,
                  float color_alpha = 1.0F,
                  Texture* texture = nullptr);
        void drawIndices(int32_t size,
                         PrimitiveType primitive_type = PrimitiveType::Triangles,
                         float color_alpha = 1.0F,
                         Texture* texture = nullptr);

        void setElementBuffer(ElementBuffer& ebo);

        void resize(size_t size) { m_vertices.resize(size); }
        void push_back(const value_type& item) { m_vertices.push_back(item); }
        void pop_back() { m_vertices.pop_back(); }
        void clear() { m_vertices.clear(); }

        value_type& front() { return m_vertices.front(); }
        value_type& back() { return m_vertices.back(); }

        [[nodiscard]] 
        size_t size() const { return m_vertices.size(); }

        void setUsage(VertexBuffer::Usage usage) { m_vbo.setUsage(usage); }

        [[nodiscard]] 
        iterator begin() { return m_vertices.begin(); } 
        [[nodiscard]] 
        iterator end() { return m_vertices.end(); }

        [[nodiscard]]
        const_iterator cbegin() const { return m_vertices.cbegin(); }
        [[nodiscard]]
        const_iterator cend() const { return m_vertices.cend(); }

        value_type& operator[](size_t index) { return m_vertices[index]; }
        const value_type& operator[](size_t index) const { return m_vertices[index]; }

        static void bind(VertexArray* vao);
        static void unbind();

    private:
        void create();
        void destroy();

        static uint32_t vao_in_bind;

        uint32_t m_id;

        VertexBuffer m_vbo;

        container m_vertices;
    };

} // namespace eg
