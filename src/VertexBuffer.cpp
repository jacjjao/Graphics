#include "../include/VertexBuffer.hpp"

#include <glad/glad.h>

#include "../include/Utility.hpp"
#include "../include/glCheck.hpp"

VertexBuffer::VertexBuffer(const size_t size) noexcept : m_vertices(size), m_cache(size), m_id{0}
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

void VertexBuffer::update() noexcept
{
    for (size_t i = 0; i < m_vertices.size(); i++)
    {
        m_cache[i].position = Utility::pointToOpenGL(m_vertices[i].position);
        m_cache[i].color    = m_vertices[i].color;
    }
    if (isAvailable())
    {
        const auto size_in_bytes = static_cast<GLsizei>(m_cache.size() * sizeof(Vertex2D));
        VertexBuffer::bind(*this);
        glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, size_in_bytes, m_cache.data()));
        VertexBuffer::unbind();
    }
}

void VertexBuffer::create() noexcept
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

void VertexBuffer::resize(const size_t size) noexcept
{
    m_vertices.resize(size);
    m_cache.resize(size);
}

void VertexBuffer::reserve(const size_t size) noexcept
{
    m_vertices.reserve(size);
    m_cache.reserve(size);
}

void VertexBuffer::push_back(const VertexBuffer::value_type& item) noexcept
{
    m_vertices.push_back(item);
    m_cache.emplace_back();
}

void VertexBuffer::pop_back() noexcept
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

size_t VertexBuffer::size() const noexcept
{
    return m_vertices.size();
}

bool VertexBuffer::isAvailable() const noexcept
{
    return m_id != 0;
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

VertexBuffer::value_type& VertexBuffer::operator[](const size_t index) noexcept
{
    return m_vertices[index];
}

const VertexBuffer::value_type& VertexBuffer::operator[](const size_t index) const noexcept
{
    return m_vertices[index];
}

void VertexBuffer::bind(const VertexBuffer& vbo) noexcept
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id));
}

void VertexBuffer::unbind() noexcept
{
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
}