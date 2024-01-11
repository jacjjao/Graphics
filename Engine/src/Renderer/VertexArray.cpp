#include "pch.hpp"
#include "include/Renderer/VertexArray.hpp"

#include "include/Renderer/ShaderProgram.hpp"
#include "include/Core/Matrix.hpp"

namespace eg
{

    uint32_t VertexArray::vao_in_bind = 0;

    VertexArray::VertexArray() :
        m_id{ 0 }
    {
        create();
    }

    VertexArray::~VertexArray()
    {
        destroy();
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
    {
        m_id       = other.m_id;

        other.m_id = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
    {
        destroy();

        m_id       = other.m_id;
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
    }

    void VertexArray::draw(const size_t count,
                           const PrimitiveType primitive_type, 
                           const float color_alpha, 
                           Texture* texture) const
    {
        auto& program = DefaultShaderProgram::instance();

        program.use();
        program.setFloat("color_alpha", color_alpha);
        if (texture != nullptr)
        {
            Texture::bind(texture);
        }
        
        VertexArray::bind(*this);
        glDrawArrays(static_cast<GLenum>(primitive_type), 0, static_cast<GLsizei>(count));
        VertexArray::unbind();
        if (texture != nullptr)
        {
            Texture::unbind();
        }
        program.unuse();
    }

    void VertexArray::drawIndices(const int32_t size,
                                  const PrimitiveType primitive_type,
                                  const float color_alpha,
                                  Texture* texture) const
    {
        auto& program = DefaultShaderProgram::instance();

        program.use();
        program.setFloat("color_alpha", color_alpha);
        if (texture != nullptr)
        {
            Texture::bind(texture);
        }

        VertexArray::bind(*this);
        glDrawElements(static_cast<GLenum>(primitive_type), size, GL_UNSIGNED_INT, 0);
        VertexArray::unbind();
        if (texture != nullptr)
        {
            Texture::unbind();
        }
        program.unuse();
    }

    void VertexArray::setElementBuffer(ElementBuffer& ebo) const
    {
        VertexArray::bind(*this);
        ElementBuffer::bind(&ebo);
        VertexArray::unbind();
        ElementBuffer::unbind();
    }

    void VertexArray::bind(const VertexArray& vao)
    {
        if (vao_in_bind != vao.m_id)
        {
            glBindVertexArray(vao.m_id);
            vao_in_bind = vao.m_id;
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
        /*
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
        */
    }

} // namespace eg