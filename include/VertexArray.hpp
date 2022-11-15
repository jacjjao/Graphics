#pragma once

#include "VertexBuffer.hpp"

class VertexArray
{
public:
    using value_type = VertexBuffer::value_type;
    using iterator = VertexBuffer::iterator;
    using const_iterator = VertexBuffer::const_iterator;
    using reverse_iterator = VertexBuffer::reverse_iterator;
    using const_reverse_iterator = VertexBuffer::const_reverse_iterator;

    explicit VertexArray(size_t size);
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) const = delete;

    void destroy();
    void create();
    void update();

    [[nodiscard]] bool isAvailable() const noexcept;
    [[nodiscard]] size_t size() const noexcept;
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

    static void bind(const VertexArray& vao);
    static void unbind();

private:
    uint32_t m_id;
    VertexBuffer m_vbo;
};
