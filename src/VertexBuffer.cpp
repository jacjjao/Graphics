#include "../include/pch.hpp"
#include "../include/VertexBuffer.hpp"

#include <glad/glad.h>

VertexBuffer* VertexBuffer::vbo_in_bind = nullptr;

VertexBuffer::VertexBuffer(const size_t size, const Usage usage) : 
m_usage{usage}, 
m_id{0},
m_size{size},
m_capacity{size}
{
    create();
}

VertexBuffer::~VertexBuffer()
{
    destroy();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
    operator=(std::move(other));
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    m_id       = other.m_id;
    m_size     = other.m_size;
    m_capacity = other.m_capacity;

    other.m_id = other.m_size = other.m_capacity = 0;

    return *this;
}

void VertexBuffer::destroy()
{
    if (isCreated())
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id   = 0;
        m_size = 0;
    }
}

void VertexBuffer::updateData(const std::span<Vertex> vertices)
{  
    glCheck(glNamedBufferSubData(m_id, 0, vertices.size() * sizeof(Vertex), vertices.data()));
}

void VertexBuffer::resize(const size_t size)
{    
    m_size = size;
    if (size <= m_capacity) 
    {
        return;
    }

    glCheck(glNamedBufferData(m_id, m_size * sizeof(Vertex), nullptr, static_cast<GLenum>(m_usage)));
}

void VertexBuffer::bind(VertexBuffer* vbo)
{
    if (vbo_in_bind != vbo)
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo->m_id));
        vbo_in_bind = vbo;
    }
}

void VertexBuffer::unbind()
{
    if (vbo_in_bind != nullptr)
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
        vbo_in_bind = nullptr;
    }
}

void VertexBuffer::create()
{
    glCheck(glCreateBuffers(1, &m_id));
    glCheck(glNamedBufferData(m_id, m_size * sizeof(Vertex), nullptr, static_cast<GLenum>(m_usage)));
}