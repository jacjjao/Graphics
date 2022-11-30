#include "../include/VertexBuffer.hpp"
#include "../include/glCheck.hpp"

#include <glad/glad.h>

VertexBuffer* VertexBuffer::vbo_in_bind = nullptr;

VertexBuffer::VertexBuffer() noexcept : m_id{}, m_usage{Usage::DYNAMIC_DRAW}
{
}

VertexBuffer::~VertexBuffer() noexcept
{
    destroy();
}

void VertexBuffer::destroy() noexcept
{
    if (isAvailable())
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id = 0;
    }
}

void VertexBuffer::updateData(const std::vector<Vertex2D>& vertices) noexcept
{
    if (isAvailable())
    {
        const auto size_in_bytes = static_cast<GLsizei>(vertices.size() * sizeof(Vertex2D));
        VertexBuffer::bind(this);
        glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_bytes, vertices.data()));
        VertexBuffer::unbind();
    }
}

void VertexBuffer::create(const std::vector<Vertex2D>& vertices) noexcept
{
    if (isAvailable())
    {
        return;
    }
    updateData(vertices);
    glCheck(glGenBuffers(1, &m_id));
    VertexBuffer::bind(this);
    const auto size_in_bytes = static_cast<GLsizei>(vertices.size() * sizeof(Vertex2D));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size_in_bytes, vertices.data(), static_cast<uint32_t>(m_usage)));
    VertexBuffer::unbind();
}

bool VertexBuffer::isAvailable() const noexcept
{
    return m_id != 0;
}

void VertexBuffer::setUsage(const Usage usage) noexcept
{
    m_usage = usage;
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