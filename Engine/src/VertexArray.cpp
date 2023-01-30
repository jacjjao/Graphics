#include "pch.hpp"
#include "../include/VertexArray.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/Matrix.hpp"

#include <glad/glad.h>

VertexArray* VertexArray::vao_in_bind = nullptr;

VertexArray::VertexArray(const size_t size, const VertexBuffer::Usage usage) :
m_vertices(size),
m_id{0},
m_vbo{size, usage}
{
    create();
}

VertexArray::~VertexArray()
{
    destroy();
}

VertexArray::VertexArray(VertexArray&& other) noexcept :
m_vbo{std::move(other.m_vbo)}
{
    operator=(std::move(other));
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_id       = other.m_id;
    m_vertices = std::move(other.m_vertices);

    other.m_id = 0;

    return *this;
}

void VertexArray::destroy()
{
    if (isCreated())
    {
        glCheck(glDeleteVertexArrays(1, &m_id));
        m_id = 0;
    }
    m_vbo.destroy();
}

void VertexArray::update()
{
    if (m_vertices.size() > m_vbo.size())
    {
        m_vbo.resize(m_vertices.size());
    }
    m_vbo.updateData(m_vertices);
}

void VertexArray::draw(const PrimitiveType primitive_type,
                       const Matrix4&      model_mat,
                       const float         color_alpha,
                       Texture* texture)
{
    auto& program = DefaultShaderProgram::instance();

    program.setMat4("model", model_mat);
    program.setFloat("color_alpha", color_alpha);
    if (texture != nullptr)
    {
        program.setI32("unit_index", texture->getUnit());
    }

    VertexArray::bind(this);
    glCheck(glDrawArrays(static_cast<GLenum>(primitive_type), 0, static_cast<GLsizei>(m_vertices.size())));
    VertexArray::unbind();
}

void VertexArray::drawIndices(const int32_t size, 
                              const PrimitiveType primitive_type, 
                              const Matrix4& model_mat, 
                              const float color_alpha, 
                              Texture* texture)
{
    auto& program = DefaultShaderProgram::instance();

    program.setMat4("model", model_mat);
    program.setFloat("color_alpha", color_alpha);
    if (texture != nullptr)
    {
        program.setI32("unit_index", texture->getUnit());
    }

    VertexArray::bind(this);
    glCheck(glDrawElements(static_cast<GLenum>(primitive_type), size, GL_UNSIGNED_INT, 0));
    VertexArray::unbind();
}

void VertexArray::setElementBuffer(ElementBuffer& ebo)
{
    VertexArray::bind(this);
    ElementBuffer::bind(&ebo);
    VertexArray::unbind();
    ElementBuffer::unbind();
}

void VertexArray::resize(const size_t size)
{
    m_vertices.resize(size);
    m_vbo.resize(size);
}

void VertexArray::bind(VertexArray* vao)
{
    if (vao_in_bind != vao)
    {
        glCheck(glBindVertexArray(vao->m_id));
        vao_in_bind = vao;
    }
}

void VertexArray::unbind()
{
    if (vao_in_bind != nullptr)
    {
        glCheck(glBindVertexArray(0));
        vao_in_bind = nullptr;
    }
}

void VertexArray::create()
{
    glCheck(glGenVertexArrays(1, &m_id));

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