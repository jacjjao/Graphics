#include "../include/VertexArray.hpp"
#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <glm/ext/vector_float2.hpp>

VertexArray::VertexArray(const size_t size) : m_vbo{size}, m_id{0}
{
}

VertexArray::~VertexArray()
{
    destroy();
}

void VertexArray::bind() const
{
    glCheck(glBindVertexArray(m_id));
}

void VertexArray::unbind()
{
    glCheck(glBindVertexArray(0));
}

void VertexArray::destroy()
{
    if (m_id != 0)
    {
        glCheck(glDeleteVertexArrays(1, &m_id));
        m_id = 0;
    }
    m_vbo.destroy();
}

void VertexArray::create()
{
    glCheck(glGenVertexArrays(1, &m_id));
    m_vbo.create();
    bind();
    m_vbo.bind();

    const auto stride = static_cast<GLsizei>(sizeof(Vertex2D));
    glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0));
    glCheck(glEnableVertexAttribArray(0));
    glCheck(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(Vertex2D::position))));
    glCheck(glEnableVertexAttribArray(1));

    unbind();
    VertexBuffer::unbind();
}

void VertexArray::update()
{
    m_vbo.update();
}

bool VertexArray::isAvailable() const
{
    return m_id != 0;
}

VertexArray::value_type& VertexArray::operator[](size_t index) noexcept
{
    return m_vbo[index];
}

const VertexArray::value_type& VertexArray::operator[](size_t index) const noexcept
{
    return m_vbo[index];
}

VertexArray::iterator VertexArray::begin() noexcept
{
    return m_vbo.begin();
}

VertexArray::iterator VertexArray::end() noexcept
{
    return m_vbo.end();
}

VertexArray::const_iterator VertexArray::cbegin() const noexcept
{
    return m_vbo.cbegin();
}

VertexArray::const_iterator VertexArray::cend() const noexcept
{
    return m_vbo.cend();
}

VertexArray::reverse_iterator VertexArray::rbegin() noexcept
{
    return m_vbo.rbegin();
}

VertexArray::reverse_iterator VertexArray::rend() noexcept
{
    return m_vbo.rend();
}

VertexArray::const_reverse_iterator VertexArray::crbegin() const noexcept
{
    return m_vbo.crbegin();
}

VertexArray::const_reverse_iterator VertexArray::crend() const noexcept
{
    return m_vbo.crend();
}