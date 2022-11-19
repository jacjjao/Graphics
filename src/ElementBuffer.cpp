#include "../include/ElementBuffer.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <utility>

ElementBuffer::ElementBuffer(const size_t size) : m_id(0), m_indices(size)
{
}

ElementBuffer::~ElementBuffer()
{
    destroy();
}

ElementBuffer& ElementBuffer::operator=(std::vector<uint32_t> indices) noexcept
{
    m_indices = std::move(indices);
    return *this;
}

ElementBuffer& ElementBuffer::operator=(const std::initializer_list<uint32_t> list) noexcept
{
    m_indices = list;
    return *this;
}

void ElementBuffer::create()
{
    if (isAvailable())
    {
        return;
    }
    glCheck(glGenBuffers(1, &m_id));
    ElementBuffer::bind(*this);
    const auto size_in_bytes = static_cast<GLsizei>(m_indices.size() * sizeof(uint32_t));
    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_in_bytes, m_indices.data(), GL_STATIC_DRAW));
    ElementBuffer::unbind();
}

void ElementBuffer::destroy()
{
    if (isAvailable())
    {
        glCheck(glDeleteBuffers(1, &m_id));
        m_id = 0;
    }
}

size_t ElementBuffer::size() const noexcept
{
    return m_indices.size();
}

bool ElementBuffer::isAvailable() const noexcept
{
    return m_id != 0;
}

void ElementBuffer::bind(const ElementBuffer& buffer)
{
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.m_id));
}

void ElementBuffer::unbind()
{
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}