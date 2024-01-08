#include "pch.hpp"
#include "include/Renderer/VertexBuffer.hpp"

namespace eg
{

    uint32_t VertexBuffer::vbo_in_bind = 0;

    VertexBuffer::VertexBuffer(const size_t size, const Usage usage) :
        m_usage{ usage },
        m_id{ 0 },
        m_size{ size },
        m_capacity{ size }
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, m_size * sizeof(element_type), nullptr, static_cast<GLenum>(m_usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        destroy();
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    {
        moveVertexBuffer(std::move(other));
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        destroy();
        moveVertexBuffer(std::move(other));
        return *this;
    }

    void VertexBuffer::moveVertexBuffer(VertexBuffer&& other) noexcept
    {
        m_id       = other.m_id;
        m_size     = other.m_size;
        m_capacity = other.m_capacity;
        m_usage    = other.m_usage;

        other.m_id   = 0;
        other.m_size = other.m_capacity = 0;
    }

    void VertexBuffer::destroy()
    {
        if (m_id > 0)
        {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
            m_size = 0;
        }
    }

    void VertexBuffer::updateData(const std::span<element_type> vertices)
    {
        glNamedBufferSubData(m_id, 0, vertices.size() * sizeof(element_type), vertices.data());
    }

    void VertexBuffer::reallocate(const size_t size)
    {
        if (size <= m_capacity)
        {
            m_size = size;
            return;
        }
        destroy(); 
        m_size = size;
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, m_size * sizeof(element_type), nullptr, static_cast<GLenum>(m_usage));
    }

    void VertexBuffer::bind(VertexBuffer* vbo)
    {
        if (vbo_in_bind != vbo->m_id)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo->m_id);
            vbo_in_bind = vbo->m_id;
        }
    }

    void VertexBuffer::unbind()
    {
        if (vbo_in_bind > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            vbo_in_bind = 0;
        }
    }

} // namespace eg