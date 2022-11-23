#include "../include/VertexArray.hpp"
#include "../include/glCheck.hpp"
#include "../include/ShaderProgram.hpp"

#include <glad/glad.h>

VertexArray::VertexArray(const size_t size) noexcept : m_vbo{size}, m_id{0}
{
}

VertexArray::~VertexArray() noexcept
{
    destroy();
}

void VertexArray::destroy() noexcept
{
    if (isAvailable())
    {
        glCheck(glDeleteVertexArrays(1, &m_id));
        m_id = 0;
    }
    m_vbo.destroy();
}

void VertexArray::create() noexcept
{
    if (isAvailable())
    {
        return;
    }

    glCheck(glGenVertexArrays(1, &m_id));
    m_vbo.create();
    VertexArray::bind(*this);
    VertexBuffer::bind(m_vbo);

    const auto stride = static_cast<GLsizei>(sizeof(Vertex2D));
    glCheck(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0));
    glCheck(glEnableVertexAttribArray(0));
    glCheck(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(Vertex2D::position))));
    glCheck(glEnableVertexAttribArray(1));
    glCheck(
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(Vertex2D::position) + sizeof(Vertex2D::color))));
    glCheck(glEnableVertexAttribArray(2));

    VertexArray::unbind();
    VertexBuffer::unbind();
}

void VertexArray::update() noexcept
{
    m_vbo.update();
}

void VertexArray::draw() noexcept
{
    if (!isAvailable())
    {
        create();
    }
    auto& program = ShaderProgram2D::instance();
    program.setMat3("model", Matrix3::identity());
    program.setBool("apply_texture", false);
    VertexArray::bind(*this);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vbo.size()));
    VertexArray::unbind();
}

void VertexArray::resize(const size_t size) noexcept
{
    m_vbo.resize(size);
}

void VertexArray::reserve(const size_t size) noexcept
{
    m_vbo.reserve(size);
}

void VertexArray::push_back(const VertexArray::value_type& item) noexcept
{
    m_vbo.push_back(item);
}

void VertexArray::pop_back() noexcept
{
    m_vbo.pop_back();
}

void VertexArray::clear() noexcept
{
    m_vbo.clear();
}

VertexArray::value_type& VertexArray::front() noexcept
{
    return m_vbo.front();
}

const VertexArray::value_type& VertexArray::front() const noexcept
{
    return m_vbo.front();
}

VertexArray::value_type& VertexArray::back() noexcept
{
    return m_vbo.back();
}

const VertexArray::value_type& VertexArray::back() const noexcept
{
    return m_vbo.back();
}

bool VertexArray::isAvailable() const noexcept
{
    return m_id != 0;
}

size_t VertexArray::size() const noexcept
{
    return m_vbo.size();
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

VertexArray::value_type& VertexArray::operator[](size_t index) noexcept
{
    return m_vbo[index];
}

const VertexArray::value_type& VertexArray::operator[](size_t index) const noexcept
{
    return m_vbo[index];
}

void VertexArray::bind(const VertexArray& vao) noexcept
{
    glCheck(glBindVertexArray(vao.m_id));
}

void VertexArray::unbind() noexcept
{
    glCheck(glBindVertexArray(0));
}