#pragma once

#include <cstdint>
#include <vector>

#include "Vertex.hpp"

class VertexBuffer
{
public:
    using value_type             = std::vector<Vertex2D>::value_type;
    using iterator               = std::vector<Vertex2D>::iterator;
    using const_iterator         = std::vector<Vertex2D>::const_iterator;
    using reverse_iterator       = std::vector<Vertex2D>::reverse_iterator;
    using const_reverse_iterator = std::vector<Vertex2D>::const_reverse_iterator;

    explicit VertexBuffer(size_t size = 0) noexcept;
    ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void destroy() noexcept;
    void update() noexcept;
    void create() noexcept;

    void resize(size_t size) noexcept;
    void reserve(size_t size) noexcept;
    void push_back(const value_type& item) noexcept;
    void pop_back() noexcept;
    void clear() noexcept;

    value_type&                     front() noexcept;
    [[nodiscard]] const value_type& front() const noexcept;

    value_type&                     back() noexcept;
    [[nodiscard]] const value_type& back() const noexcept;

    [[nodiscard]] size_t size() const noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

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

    static void bind(const VertexBuffer& vbo) noexcept;
    static void unbind() noexcept;

private:
    std::vector<Vertex2D> m_vertices;
    std::vector<Vertex2D> m_cache;
    uint32_t              m_id;
};
