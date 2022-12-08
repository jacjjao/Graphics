#pragma once

#include <cstdint>
#include <vector>

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

    void destroy() noexcept;
    void updateData(const std::vector<Vertex>& vertices) noexcept;
    void create(const std::vector<Vertex>& vertices) noexcept;

    [[nodiscard]] bool isAvailable() const noexcept;

    void setUsage(Usage usage) noexcept;

    static void bind(VertexBuffer* vbo) noexcept;
    static void unbind() noexcept;

private:
    static VertexBuffer* vbo_in_bind;

    Usage    m_usage;
    uint32_t m_id;
};
