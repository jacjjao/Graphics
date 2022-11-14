#pragma once

#include "Vertex.hpp"
#include <cstdint>
#include <vector>

class VertexBuffer
{
public:
    using vector_type = std::vector<Vertex2D>;
    using value_type = vector_type::value_type;
    using iterator = vector_type::iterator;
    using const_iterator = vector_type::const_iterator;
    using reverse_iterator = vector_type::reverse_iterator;
    using const_reverse_iterator = vector_type::const_reverse_iterator;

    explicit VertexBuffer(size_t size);
    ~VertexBuffer();

    VertexBuffer(VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&) = delete;

    void bind() const;
    void destroy();
    void update();
    void create();

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool is_available() const;
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

    static void unbind();

private:
    vector_type m_vertices;
    uint32_t m_id;
};
