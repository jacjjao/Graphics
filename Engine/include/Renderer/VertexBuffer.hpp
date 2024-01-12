#pragma once

#include <cstdint>
#include <span>
#include <glad/glad.h>
#include "include/Core/Vertex.hpp"
#include "include/Core/Core.hpp"

namespace eg
{

    enum class EG_API VertexBufferUsage : uint32_t
    {
        StreamDraw  = 0x88E0,
        StaticDraw  = 0x88E4,
        DynamicDraw = 0x88E8
    };

    template<typename T>
    class EG_API VertexBuffer
    {
    private:
        using container = std::vector<T>;
        using iterator = container::iterator;
        using const_iterator = container::const_iterator;

    public:
        explicit VertexBuffer() = delete;

        explicit VertexBuffer(size_t size, VertexBufferUsage usage = VertexBufferUsage::StaticDraw) :
        m_usage{usage},
        m_id{0},
        m_vertices(size)
        {
            glCreateBuffers(1, &m_id);
            glNamedBufferData(m_id, m_vertices.size() * sizeof(T), nullptr, static_cast<GLenum>(m_usage));
        }

        ~VertexBuffer()
        {
            destroy();
        }

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept
        {
            moveVertexBuffer(std::move(other));
        }

        VertexBuffer& operator=(VertexBuffer&& other) noexcept
        {
            destroy();
            moveVertexBuffer(std::move(other));
            return *this;
        }

        void destroy()
        {
            if (m_id > 0)
            {
                glDeleteBuffers(1, &m_id);
                m_id   = 0;
            }
        }

        void update()
        {
            glNamedBufferSubData(m_id, 0, m_vertices.size() * sizeof(T), m_vertices.data());
        }

        void resize(size_t size)
        {
            if (size <= m_vertices.size())
                return;
            destroy();
            m_vertices.resize(size);
            glCreateBuffers(1, &m_id);
            glNamedBufferData(m_id, m_vertices.size() * sizeof(T), nullptr, static_cast<GLenum>(m_usage));
        }

        [[nodiscard]] 
        size_t size() const { return m_vertices.size(); }
        [[nodiscard]] 
        size_t getCapacity() const { return m_vertices.capacity(); }

        [[nodiscard]] iterator begin()
        {
            return m_vertices.begin();
        }

        [[nodiscard]] iterator end()
        {
            return m_vertices.end();
        }

        [[nodiscard]] iterator cbegin()
        {
            return m_vertices.cbegin();
        }

        [[nodiscard]] iterator cend()
        {
            return m_vertices.cend();
        }

        void setUsage(VertexBufferUsage usage) { m_usage = usage; }

        T& operator[](const size_t index)
        {
            return m_vertices[index];
        }

        const T& operator[](const size_t index) const
        {
            return m_vertices[index];
        }

        static void bind(const VertexBuffer& vbo)
        {
            if (vbo_in_bind != vbo.m_id)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id);
                vbo_in_bind = vbo.m_id;
            }
        }

        static void unbind()
        {
            if (vbo_in_bind > 0)
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                vbo_in_bind = 0;
            }
        }

    private:
        void moveVertexBuffer(VertexBuffer&& other) noexcept
        {
            m_vertices = std::move(other.m_vertices);
            m_id       = other.m_id;
            m_usage    = other.m_usage;

            other.m_id   = 0;
        }

        static inline uint32_t vbo_in_bind = 0;

        VertexBufferUsage m_usage;

        uint32_t m_id;

        std::vector<T> m_vertices;
    };

} // namespace eg
