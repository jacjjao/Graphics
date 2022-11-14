#include "../include/VertexBuffer.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const size_t size) : m_vertices(size), m_id{0}
{
}

VertexBuffer::~VertexBuffer()
{
    destroy();
}

void VertexBuffer::create()
{
    glCheck(glGenBuffers(1, &m_id));
    bind();
    const auto size_in_bytes = static_cast<GLsizei>(m_vertices.size() * sizeof(Vertex2D));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size_in_bytes, m_vertices.data(), GL_STREAM_DRAW));
    unbind();
}

bool VertexBuffer::is_available() const
{
    return m_id != 0;
}

VertexBuffer::value_type& VertexBuffer::operator[](const size_t index) noexcept
{
    return m_vertices[index];
}

const VertexBuffer::value_type& VertexBuffer::operator[](const size_t index) const noexcept
{
    return m_vertices[index];
}

size_t VertexBuffer::size() const noexcept
{
    return m_vertices.size();
}

void VertexBuffer::bind() const
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::unbind()
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::destroy()
{
    if (m_id != 0)
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id = 0;
    }
}

void VertexBuffer::update()
{
    const auto size_in_bytes = static_cast<GLsizei>(m_vertices.size() * sizeof(Vertex2D));
    bind();
    glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_bytes, m_vertices.data()));
    unbind();
}

VertexBuffer::iterator VertexBuffer::begin() noexcept
{
    return m_vertices.begin();
}

VertexBuffer::iterator VertexBuffer::end() noexcept
{
    return m_vertices.end();
}

VertexBuffer::const_iterator VertexBuffer::cbegin() const noexcept
{
    return m_vertices.cbegin();
}

VertexBuffer::const_iterator VertexBuffer::cend() const noexcept
{
    return m_vertices.cend();
}

VertexBuffer::reverse_iterator VertexBuffer::rbegin() noexcept
{
    return m_vertices.rbegin();
}

VertexBuffer::reverse_iterator VertexBuffer::rend() noexcept
{
    return m_vertices.rend();
}

VertexBuffer::const_reverse_iterator VertexBuffer::crbegin() const noexcept
{
    return m_vertices.crbegin();
}

VertexBuffer::const_reverse_iterator VertexBuffer::crend() const noexcept
{
    return m_vertices.crend();
}