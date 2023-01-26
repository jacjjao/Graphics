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

    VertexBuffer(size_t size, Usage usage = Usage::StaticDraw) noexcept;
    ~VertexBuffer() noexcept;

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void destroy() noexcept;
    void updateData(std::span<Vertex> vertices) noexcept;
    void resize(size_t size) noexcept;

    [[nodiscard]] size_t size() const noexcept { return m_size; }
    [[nodiscard]] size_t getCapacity() const noexcept { return m_capacity; }

    [[nodiscard]] bool isCreated() const noexcept { return m_id > 0; }

    void setUsage(Usage usage) noexcept { m_usage = usage; }

    static void bind(VertexBuffer* vbo) noexcept;
    static void unbind() noexcept;

private:
    void create() noexcept;

    static VertexBuffer* vbo_in_bind;

    Usage m_usage;

    uint32_t m_id;

    size_t m_size;
    size_t m_capacity;
};
