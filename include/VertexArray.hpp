#pragma once

#include "VertexBuffer.hpp"
#include "PrimitiveType.hpp"

#include <vector>

class VertexArray
{
public:
    using value_type             = std::vector<Vertex>::value_type;
    using iterator               = std::vector<Vertex>::iterator;
    using const_iterator         = std::vector<Vertex>::const_iterator;
    using reverse_iterator       = std::vector<Vertex>::reverse_iterator;
    using const_reverse_iterator = std::vector<Vertex>::const_reverse_iterator;

    explicit VertexArray(size_t size = 0) noexcept;
    ~VertexArray() noexcept;

    VertexArray(const VertexArray&)                  = delete;
    VertexArray& operator=(const VertexArray&) const = delete;

    void destroy() noexcept;
    void create() noexcept;
    void update() noexcept;
    void draw(PrimitiveType primitive_type = PrimitiveType::Triangles) noexcept;

    void resize(size_t size) noexcept;
    void reserve(size_t size) noexcept;
    void push_back(const value_type& item) noexcept;
    void pop_back() noexcept;
    void clear() noexcept;

    value_type&                     front() noexcept;
    [[nodiscard]] const value_type& front() const noexcept;

    value_type&                     back() noexcept;
    [[nodiscard]] const value_type& back() const noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

    [[nodiscard]] size_t size() const noexcept;

    void setUsage(VertexBuffer::Usage usage) noexcept;

    [[nodiscard]] iterator               begin() noexcept;
    [[nodiscard]] iterator               end() noexcept;
    [[nodiscard]] const_iterator         cbegin() const noexcept;
    [[nodiscard]] const_iterator         cend() const noexcept;
    [[nodiscard]] reverse_iterator       rbegin() noexcept;
    [[nodiscard]] reverse_iterator       rend() noexcept;
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    value_type&       operator[](size_t index) noexcept;
    const value_type& operator[](size_t index) const noexcept;

    static void bind(VertexArray* vao) noexcept;
    static void unbind() noexcept;

private:
    void transformData() noexcept;

    static std::vector<Vertex> cache;
    static VertexArray*        vao_in_bind;

    uint32_t m_id;

    VertexBuffer        m_vbo;

    std::vector<Vertex> m_vertices;
};
