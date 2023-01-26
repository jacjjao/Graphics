#include "../include/pch.hpp"
#include "../include/VertexBuffer.hpp"

#include <glad/glad.h>

VertexBuffer* VertexBuffer::vbo_in_bind = nullptr;

VertexBuffer::VertexBuffer(const size_t size, const Usage usage) noexcept : 
m_usage{usage}, 
m_id{0},
m_size{size},
m_capacity{size}
{
    create();
}

VertexBuffer::~VertexBuffer() noexcept
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

void VertexBuffer::destroy() noexcept
{
    if (isCreated())
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id   = 0;
        m_size = 0;
    }
}

void VertexBuffer::updateData(const std::span<Vertex> vertices) noexcept
{  
    VertexBuffer::bind(this);

    glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

    VertexBuffer::unbind();
}

void VertexBuffer::resize(const size_t size) noexcept
{    
    m_size = size;
    if (size <= m_capacity) 
    {
        return;
    }

    VertexBuffer::bind(this);

    glCheck(glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(Vertex), nullptr, static_cast<GLenum>(m_usage)));

    VertexBuffer::unbind();
}

void VertexBuffer::bind(VertexBuffer* vbo) noexcept
{
    if (vbo_in_bind != vbo)
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo->m_id));
        vbo_in_bind = vbo;
    }
}

void VertexBuffer::unbind() noexcept
{
    if (vbo_in_bind != nullptr)
    {
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
        vbo_in_bind = nullptr;
    }
}

void VertexBuffer::create() noexcept
{
    glCheck(glGenBuffers(1, &m_id));

    VertexBuffer::bind(this);

    glCheck(glBufferData(GL_ARRAY_BUFFER, m_size * sizeof(Vertex), nullptr, static_cast<GLenum>(m_usage)));

    VertexBuffer::unbind();
}