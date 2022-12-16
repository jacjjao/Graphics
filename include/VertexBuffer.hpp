#pragma once

#include <cstdint>
#include <span>

#include "Vertex.hpp"

class VertexBuffer
{
public:
    enum class Usage : uint32_t
    {
        StreamDraw  = 0x88E0,
        StaticDraw  = 0x88E4,
        DynamicDraw = 0x88E8
    };

    explicit VertexBuffer() noexcept;
    ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;

    void destroy() noexcept;
    void updateData(std::span<Vertex> vertices) noexcept;
    void create(std::span<Vertex> vertices) noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

    void setUsage(Usage usage) noexcept;

    static void bind(VertexBuffer* vbo) noexcept;
    static void unbind() noexcept;

private:
    static VertexBuffer* vbo_in_bind;

    Usage m_usage;

    uint32_t m_id;

    size_t m_size;

    int32_t m_size_in_bytes;
};
