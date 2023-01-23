#include "../include/pch.hpp"
#include "../include/VertexArray.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/Matrix.hpp"

#include <glad/glad.h>

std::vector<Vertex> VertexArray::cache(40);
VertexArray*        VertexArray::vao_in_bind = nullptr;

VertexArray::VertexArray(const size_t size) noexcept :
m_vertices(size),
m_id{0}
{
}

VertexArray::~VertexArray() noexcept
{
    destroy();
}

void VertexArray::destroy() noexcept
{
    if (isCreated())
    {
        glCheck(glDeleteVertexArrays(1, &m_id));
        m_id = 0;
    }
    m_vbo.destroy();
}

void VertexArray::create() noexcept
{
    if (isCreated())
    {
        return;
    }

    glCheck(glGenVertexArrays(1, &m_id));


    sendDataToCache();
    m_vbo.create(cache);

    VertexArray::bind(this);
    VertexBuffer::bind(&m_vbo);

    constexpr auto stride = static_cast<GLsizei>(sizeof(Vertex));
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0));
    glCheck(glEnableVertexAttribArray(0));

    glCheck(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(Vertex::position))));
    glCheck(glEnableVertexAttribArray(1));

    glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(Vertex::position) + sizeof(Vertex::color))));
    glCheck(glEnableVertexAttribArray(2));

    VertexArray::unbind();
    VertexBuffer::unbind();
}

void VertexArray::update() noexcept
{
    sendDataToCache();
    m_vbo.updateData(cache);
}

void VertexArray::draw(const PrimitiveType primitive_type,
                       const Matrix4&      model_mat,
                       const float         color_alpha) noexcept
{
    auto& program = DefaultShaderProgram::instance();

    program.setMat4("model", model_mat);
    program.setFloat("color_alpha", color_alpha);

    VertexArray::bind(this);
    glCheck(glDrawArrays(static_cast<GLenum>(primitive_type), 0, static_cast<GLsizei>(m_vertices.size())));
    VertexArray::unbind();
}

void VertexArray::resize(const size_t size) noexcept
{
    m_vertices.resize(size);
}

void VertexArray::reserve(const size_t size) noexcept
{
    m_vertices.reserve(size);
}

void VertexArray::push_back(const VertexArray::value_type& item) noexcept
{
    m_vertices.push_back(item);
}

void VertexArray::pop_back() noexcept
{
    m_vertices.pop_back();
}

void VertexArray::clear() noexcept
{
    m_vertices.clear();
}

VertexArray::value_type& VertexArray::front() noexcept
{
    return m_vertices.front();
}

const VertexArray::value_type& VertexArray::front() const noexcept
{
    return m_vertices.front();
}

VertexArray::value_type& VertexArray::back() noexcept
{
    return m_vertices.back();
}

const VertexArray::value_type& VertexArray::back() const noexcept
{
    return m_vertices.back();
}

bool VertexArray::isCreated() const noexcept
{
    return m_id != 0;
}

size_t VertexArray::size() const noexcept
{
    return m_vertices.size();
}

void VertexArray::setUsage(const VertexBuffer::Usage usage) noexcept
{
    m_vbo.setUsage(usage);
}

VertexArray::iterator VertexArray::begin() noexcept
{
    return m_vertices.begin();
}

VertexArray::iterator VertexArray::end() noexcept
{
    return m_vertices.end();
}

VertexArray::const_iterator VertexArray::cbegin() const noexcept
{
    return m_vertices.cbegin();
}

VertexArray::const_iterator VertexArray::cend() const noexcept
{
    return m_vertices.cend();
}

VertexArray::reverse_iterator VertexArray::rbegin() noexcept
{
    return m_vertices.rbegin();
}

VertexArray::reverse_iterator VertexArray::rend() noexcept
{
    return m_vertices.rend();
}

VertexArray::const_reverse_iterator VertexArray::crbegin() const noexcept
{
    return m_vertices.crbegin();
}

VertexArray::const_reverse_iterator VertexArray::crend() const noexcept
{
    return m_vertices.crend();
}

VertexArray::value_type& VertexArray::operator[](size_t index) noexcept
{
    return m_vertices[index];
}

const VertexArray::value_type& VertexArray::operator[](size_t index) const noexcept
{
    return m_vertices[index];
}

void VertexArray::bind(VertexArray* vao) noexcept
{
    if (vao_in_bind != vao)
    {
        glCheck(glBindVertexArray(vao->m_id));
        vao_in_bind = vao;
    }
}

void VertexArray::unbind() noexcept
{
    if (vao_in_bind != nullptr)
    {
        glCheck(glBindVertexArray(0));
        vao_in_bind = nullptr;
    }
}

void VertexArray::sendDataToCache() noexcept
{
    cache.resize(m_vertices.size());
    for (int i = 0; i < m_vertices.size(); i++)
    {
        cache[i] = m_vertices[i];
    }
}
