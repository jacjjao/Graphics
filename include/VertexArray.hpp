#pragma once

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"
#include "PrimitiveType.hpp"
#include "Matrix.hpp"
#include "Texture.hpp"

#include <vector>

class VertexArray
{
public:
    using value_type = std::vector<Vertex>::value_type;
    using iterator   = std::vector<Vertex>::iterator;

    explicit VertexArray(size_t size, VertexBuffer::Usage usage = VertexBuffer::Usage::StaticDraw) noexcept;
    ~VertexArray() noexcept;

    VertexArray(const VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) const = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void destroy() noexcept;
    void update() noexcept;
    void draw(PrimitiveType primitive_type = PrimitiveType::Triangles,
              const Matrix4& model_mat = Constants::identity_mat4,
              float color_alpha = 1.0F, 
              Texture* texture = nullptr) noexcept;
    void drawIndices(int32_t size,
                     PrimitiveType primitive_type = PrimitiveType::Triangles,
                     const Matrix4& model_mat = Constants::identity_mat4,
                     float color_alpha = 1.0F,
                     Texture* texture = nullptr) noexcept;

    void setElementBuffer(ElementBuffer& ebo) noexcept;

    void resize(size_t size) noexcept;
    void push_back(const value_type& item) noexcept { m_vertices.push_back(item); }
    void pop_back() noexcept { m_vertices.pop_back(); }
    void clear() noexcept { m_vertices.clear(); }

    value_type& front() noexcept { return m_vertices.front(); }
    value_type& back() noexcept { return m_vertices.back(); }

    [[nodiscard]] bool isCreated() const noexcept { return m_id > 0; }
    [[nodiscard]] size_t size() const noexcept { return m_vertices.size(); }

    void setUsage(VertexBuffer::Usage usage) noexcept { m_vbo.setUsage(usage); }

    [[nodiscard]] iterator begin() noexcept { return m_vertices.begin(); }
    [[nodiscard]] iterator end() noexcept { return m_vertices.end(); }

    value_type&       operator[](size_t index) noexcept { return m_vertices[index]; }
    const value_type& operator[](size_t index) const noexcept { return m_vertices[index]; }

    static void bind(VertexArray* vao) noexcept;
    static void unbind() noexcept;

private:
    void create();

    static VertexArray* vao_in_bind;

    uint32_t m_id;

    VertexBuffer m_vbo;

    std::vector<Vertex> m_vertices;
};
