#include "../include/VertexBuffer.hpp"
#include "../include/glCheck.hpp"

#include <glad/glad.h>

VertexBuffer* VertexBuffer::vbo_in_bind = nullptr;

VertexBuffer::VertexBuffer() noexcept : m_id{}, m_usage{Usage::DynamicDraw}, m_size{0}, m_size_in_bytes{0}
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
        m_id   = 0;
        m_size = m_size_in_bytes = 0;
    }
}

void VertexBuffer::updateData(const std::vector<Vertex>& vertices) noexcept
{
    if (!isAvailable())
    {
        create(vertices);
        return;
    }

    if (vertices.empty())
    {
        destroy();
        return;
    }

    if (vertices.size() != m_size)
    {
        create(vertices);
        return;
    }

    VertexBuffer::bind(this);

    glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, m_size_in_bytes, vertices.data()));

    VertexBuffer::unbind();
}

void VertexBuffer::create(const std::vector<Vertex>& vertices) noexcept
{
    if (vertices.empty())
    {
        return;
    }

    if (isAvailable())
    {
        destroy();
    }

    m_size          = vertices.size();
    m_size_in_bytes = static_cast<GLsizei>(m_size * sizeof(Vertex));

    glCheck(glGenBuffers(1, &m_id));

    VertexBuffer::bind(this);

    glCheck(glBufferData(GL_ARRAY_BUFFER, m_size_in_bytes, vertices.data(), static_cast<GLenum>(m_usage)));

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