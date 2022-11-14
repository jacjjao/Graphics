#include "../include/ElementBuffer.hpp"
#include "../include/glCheck.hpp"
#include <cassert>
#include <glad/glad.h>

ElementBuffer::ElementBuffer(const size_t size) : m_id(0), m_indices(size)
{
}

ElementBuffer::~ElementBuffer()
{
    destroy();
}

void ElementBuffer::bind() const
{
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void ElementBuffer::create()
{
    glCheck(glGenBuffers(1, &m_id));
    bind();
    const auto size_in_bytes = static_cast<GLsizei>(m_indices.size() * sizeof(uint32_t));
    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_in_bytes, m_indices.data(), GL_STATIC_DRAW));
    unbind();
}

void ElementBuffer::destroy()
{
    if (m_id != 0)
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id = 0;
    }
}

size_t ElementBuffer::size() const noexcept
{
    return m_indices.size();
}

void ElementBuffer::unbind()
{
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

ElementBuffer& ElementBuffer::operator=(const std::initializer_list<uint32_t> list)
{
    assert(m_indices.size() == list.size());
    m_indices.assign(list.begin(), list.end());
    return *this;
}