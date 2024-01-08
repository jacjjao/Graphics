#include "pch.hpp"
#include "include/Renderer/VertexArray.hpp"

#include "include/Renderer/ShaderProgram.hpp"
#include "include/Core/Matrix.hpp"

namespace eg
{

    uint32_t VertexArray::vao_in_bind = 0;

    VertexArray::VertexArray(const size_t size, const VertexBuffer::Usage usage) :
        m_vertices(size),
        m_id{ 0 },
        m_vbo{ size, usage }
    {
        create();
    }

    VertexArray::~VertexArray()
    {
        destroy();
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept :
        m_vbo{ std::move(other.m_vbo) }
    {
        m_id       = other.m_id;
        m_vertices = std::move(other.m_vertices);

        other.m_id = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
    {
        destroy();

        m_id       = other.m_id;
        m_vertices = std::move(other.m_vertices);
        m_vbo      = std::move(other.m_vbo);

        other.m_id = 0;

        return *this;
    }

    void VertexArray::destroy()
    {
        if (m_id != 0)
        {
            glDeleteVertexArrays(1, &m_id);
            m_id = 0;
        }
        m_vbo.destroy();
    }

    void VertexArray::update()
    {
        if (m_vertices.size() > m_vbo.size())
        {
            m_vbo.reallocate(m_vertices.size());
        }
        m_vbo.updateData(m_vertices);
    }

    void VertexArray::draw(const PrimitiveType primitive_type,
                           const Matrix4& model_mat,
                           const float color_alpha,
                           Texture* texture)
    {
        auto& program = DefaultShaderProgram::instance();

        program.use();
        program.setMat4("model", model_mat);
        program.setFloat("color_alpha", color_alpha);
        if (texture != nullptr)
        {
            Texture::bind(texture);
        }

        VertexArray::bind(this);
        glDrawArrays(static_cast<GLenum>(primitive_type), 0, static_cast<GLsizei>(m_vertices.size()));
        VertexArray::unbind();
        if (texture != nullptr)
        {
            Texture::unbind();
        }
        program.unuse();
    }

    void VertexArray::drawIndices(const int32_t size,
                                  const PrimitiveType primitive_type,
                                  const Matrix4& model_mat,
                                  const float color_alpha,
                                  Texture* texture)
    {
        auto& program = DefaultShaderProgram::instance();

        program.use();
        program.setMat4("model", model_mat);
        program.setFloat("color_alpha", color_alpha);
        if (texture != nullptr)
        {
            Texture::bind(texture);
        }

        VertexArray::bind(this);
        glDrawElements(static_cast<GLenum>(primitive_type), size, GL_UNSIGNED_INT, 0);
        VertexArray::unbind();
        if (texture != nullptr)
        {
            Texture::unbind();
        }
        program.unuse();
    }

    void VertexArray::setElementBuffer(ElementBuffer& ebo)
    {
        VertexArray::bind(this);
        ElementBuffer::bind(&ebo);
        VertexArray::unbind();
        ElementBuffer::unbind();
    }

    void VertexArray::bind(VertexArray* vao)
    {
        if (vao_in_bind != vao->m_id)
        {
            glBindVertexArray(vao->m_id);
            vao_in_bind = vao->m_id;
        }
    }

    void VertexArray::unbind()
    {
        if (vao_in_bind != 0)
        {
            glBindVertexArray(0);
            vao_in_bind = 0;
        }
    }

    void VertexArray::create()
    {
        glGenVertexArrays(1, &m_id);

        VertexArray::bind(this);
        VertexBuffer::bind(&m_vbo);

        constexpr auto stride = static_cast<GLsizei>(sizeof(element_type));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (void*)(sizeof(element_type::position)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(element_type::position) + sizeof(element_type::color)));
        glEnableVertexAttribArray(2);

        VertexArray::unbind();
        VertexBuffer::unbind();
    }

} // namespace eg