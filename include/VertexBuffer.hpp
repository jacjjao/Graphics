#pragma once

#include <cstdint>
#include <vector>

#include "Vertex.hpp"

class VertexBuffer
{
public:
    explicit VertexBuffer() noexcept;
    ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void destroy() noexcept;
    void updateData(const std::vector<Vertex2D>& vertices) const noexcept;
    void create(const std::vector<Vertex2D>& vertices) noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

    static void bind(const VertexBuffer& vbo) noexcept;
    static void unbind() noexcept;

private:
    uint32_t m_id;
};
