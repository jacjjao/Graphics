#include "pch.hpp"
#include "include/Renderer/ElementBuffer.hpp"

#include <glad/glad.h>

namespace Engine
{

    uint32_t ElementBuffer::ebo_in_bind = 0;

    ElementBuffer::ElementBuffer(const size_t size) :
        m_id(0),
        m_indices(size)
    {
        glCreateBuffers(1, &m_id);

        const auto size_in_bytes = static_cast<GLsizei>(m_indices.size() * sizeof(uint32_t));
        glNamedBufferData(m_id, size_in_bytes, nullptr, GL_STATIC_DRAW);
    }

    ElementBuffer::~ElementBuffer()
    {
        destroy();
    }

    ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept
    {
        operator=(std::move(other));
    }

    ElementBuffer& ElementBuffer::operator=(ElementBuffer&& other) noexcept
    {
        destroy();

        m_id = other.m_id;
        m_indices = std::move(other.m_indices);

        other.m_id = 0;

        return *this;
    }

    ElementBuffer& ElementBuffer::operator=(std::vector<uint32_t> indices)
    {
        m_indices = std::move(indices);
        return *this;
    }

    ElementBuffer& ElementBuffer::operator=(const std::initializer_list<uint32_t> list)
    {
        m_indices = list;
        return *this;
    }

    void ElementBuffer::update()
    {
        glNamedBufferSubData(m_id, 0, m_indices.size() * sizeof(uint32_t), m_indices.data());
    }

    void ElementBuffer::destroy()
    {
        if (m_id > 0)
        {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
        }
    }

    void ElementBuffer::bind(ElementBuffer* ebo)
    {
        if (ebo_in_bind != ebo->m_id)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->m_id);
            ebo_in_bind = ebo->m_id;
        }
    }

    void ElementBuffer::unbind()
    {
        if (ebo_in_bind != 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ebo_in_bind = 0;
        }
    }

} // namespace Engine