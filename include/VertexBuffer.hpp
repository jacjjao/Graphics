#pragma once

#include "Vertex.hpp"
#include <cstdint>
#include <vector>

class VertexBuffer
{
public:
    using value_type = std::vector<Vertex2D>::value_type;
    using iterator = std::vector<Vertex2D>::iterator;
    using const_iterator = std::vector<Vertex2D>::const_iterator;
    using reverse_iterator = std::vector<Vertex2D>::reverse_iterator;
    using const_reverse_iterator = std::vector<Vertex2D>::const_reverse_iterator;

    explicit VertexBuffer(size_t size = 0);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void destroy();
    void update();
    void create();

    void resize(size_t size);
    void reserve(size_t size);
    void push_back(const value_type& item);
    void pop_back();
    void clear() noexcept;

    value_type& front() noexcept;
    [[nodiscard]] const value_type& front() const noexcept;

    value_type& back() noexcept;
    [[nodiscard]] const value_type& back() const noexcept;

    [[nodiscard]] size_t size() const noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] iterator end() noexcept;
    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;
    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    value_type& operator[](size_t index) noexcept;
    const value_type& operator[](size_t index) const noexcept;

    static void bind(const VertexBuffer& vbo);
    static void unbind();

private:
    std::vector<Vertex2D> m_vertices;
    std::vector<Vertex2D> m_cache;
    uint32_t m_id;
};
