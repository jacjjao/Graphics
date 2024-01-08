#pragma once

#include <cstdint>
#include <span>

#include "include/Core/Vertex.hpp"

namespace eg
{

    class VertexBuffer
    {
    private:
        using element_type = Vertex2D;

    public:
        enum class Usage : uint32_t
        {
            StreamDraw = 0x88E0,
            StaticDraw = 0x88E4,
            DynamicDraw = 0x88E8
        };

        VertexBuffer(size_t size, Usage usage = Usage::StaticDraw);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        void destroy();
        void updateData(std::span<element_type> vertices);
        void reallocate(size_t size);

        [[nodiscard]] 
        size_t size() const { return m_size; }
        [[nodiscard]] 
        size_t getCapacity() const { return m_capacity; }

        void setUsage(Usage usage) { m_usage = usage; }

        static void bind(VertexBuffer* vbo);
        static void unbind();

    private:
        void moveVertexBuffer(VertexBuffer&& other) noexcept;

        static uint32_t vbo_in_bind;

        Usage m_usage;

        uint32_t m_id;

        size_t m_size;
        size_t m_capacity;
    };

} // namespace eg
