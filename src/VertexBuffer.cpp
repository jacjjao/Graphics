#include "../include/VertexBuffer.hpp"
#include "../include/glCheck.hpp"
#include "../include/Utility.hpp"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const size_t size) : m_vertices(size), m_cache(size), m_id{0}
{
}

VertexBuffer::~VertexBuffer()
{
    destroy();
}

void VertexBuffer::create()
{
    if (isAvailable())
    {
        return;
    }
    update();
    glCheck(glGenBuffers(1, &m_id));
    VertexBuffer::bind(*this);
    const auto size_in_bytes = static_cast<GLsizei>(m_cache.size() * sizeof(Vertex2D));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size_in_bytes, m_cache.data(), GL_DYNAMIC_DRAW));
    VertexBuffer::unbind();
}

bool VertexBuffer::isAvailable() const noexcept
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

void VertexBuffer::bind(const VertexBuffer& vbo)
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id));
}

void VertexBuffer::unbind()
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::destroy()
{
    if (isAvailable())
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id = 0;
    }
}

void VertexBuffer::update()
{
    for (size_t i = 0; i < m_vertices.size(); i++)
    {
        m_cache[i].position = util::pointToOpenGL(m_vertices[i].position);
        m_cache[i].color = m_vertices[i].color;
    }
    if (isAvailable())
    {
        const auto size_in_bytes = static_cast<GLsizei>(m_cache.size() * sizeof(Vertex2D));
        VertexBuffer::bind(*this);
        glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_bytes, m_cache.data()));
        VertexBuffer::unbind();
    }
}

void VertexBuffer::resize(const size_t size)
{
    m_vertices.resize(size);
    m_cache.resize(size);
}

void VertexBuffer::reserve(const size_t size)
{
    m_vertices.reserve(size);
    m_cache.reserve(size);
}

void VertexBuffer::push_back(const VertexBuffer::value_type& item)
{
    m_vertices.push_back(item);
    m_cache.emplace_back();
}

void VertexBuffer::pop_back()
{
    m_vertices.pop_back();
    m_cache.pop_back();
}

void VertexBuffer::clear() noexcept
{
    m_vertices.clear();
    m_cache.clear();
}

VertexBuffer::value_type& VertexBuffer::front() noexcept
{
    return m_vertices.front();
}

const VertexBuffer::value_type& VertexBuffer::front() const noexcept
{
    return m_vertices.front();
}

VertexBuffer::value_type& VertexBuffer::back() noexcept
{
    return m_vertices.back();
}

const VertexBuffer::value_type& VertexBuffer::back() const noexcept
{
    return m_vertices.back();
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