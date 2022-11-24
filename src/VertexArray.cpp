#include "../include/VertexArray.hpp"
#include "../include/glCheck.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Utility.hpp"

#include <glad/glad.h>

std::vector<Vertex2D> VertexArray::cache(40);

VertexArray::VertexArray(const size_t size) noexcept : m_vertices(size), m_id{0}, m_texture{nullptr}
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
    transformData();
    m_vbo.create(cache);
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
    transformData();
    m_vbo.updateData(cache);
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
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
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

bool VertexArray::isAvailable() const noexcept
{
    return m_id != 0;
}

void VertexArray::applyTexture(Texture* texture) noexcept
{
    m_texture = texture;
}

Texture* VertexArray::getTexture() noexcept
{
    return m_texture;
}

const Texture* VertexArray::getTexture() const noexcept
{
    return m_texture;
}

bool VertexArray::hasTexture() const noexcept
{
    return m_texture != nullptr;
}

size_t VertexArray::size() const noexcept
{
    return m_vertices.size();
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

void VertexArray::bind(const VertexArray& vao) noexcept
{
    glCheck(glBindVertexArray(vao.m_id));
}

void VertexArray::unbind() noexcept
{
    glCheck(glBindVertexArray(0));
}

void VertexArray::transformData() noexcept
{
    cache.resize(m_vertices.size());
    for (int i = 0; i < m_vertices.size(); i++)
    {
        cache[i].position = Utility::pointToOpenGL(m_vertices[i].position);
        cache[i].color    = m_vertices[i].color;
    }
    if (hasTexture())
    {
        for (int i = 0; i < m_vertices.size(); i++)
        {
            const Vector2f size = {static_cast<float>(m_texture->getWidth()), static_cast<float>(m_texture->getHeight())};
            cache[i].tex_coord = Utility::pointToTexCoord(m_vertices[i].tex_coord, size);
        }
    }
}