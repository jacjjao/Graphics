#include "../include/Texture.hpp"
#include "../include/glCheck.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

Texture* Texture::texture_in_bind = nullptr;

Texture::Texture(const std::string& path) noexcept : m_id{}, m_size{}
{
    // create texture
    glCheck(glGenTextures(1, &m_id));
    bind(this);

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // load image
    int width      = 0;
    int height     = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(true);

    uint8_t* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data != nullptr)
    {
        m_size.x = static_cast<float>(width);
        m_size.y = static_cast<float>(height);
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << '\n';
    }
    stbi_image_free(data);
}

Texture::~Texture() noexcept
{
    destroy();
}

void Texture::destroy() noexcept
{
    if (m_id != 0)
    {
        glCheck(glDeleteTextures(1, &m_id));
        m_id = 0;
    }
}

Vector2f Texture::getSize() const noexcept
{
    return m_size;
}

float Texture::getWidth() const noexcept
{
    return m_size.x;
}

float Texture::getHeight() const noexcept
{
    return m_size.y;
}

void Texture::bind(Texture* texture) noexcept
{
    if (texture_in_bind != texture)
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_id));
        texture_in_bind = texture;
    }
}

void Texture::unbind() noexcept
{
    if (texture_in_bind != nullptr)
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
        texture_in_bind = nullptr;
    }
}

Vector2f Texture::pointToTexCoord(const Vector2f point, const Vector2f tex_size) noexcept
{
    Vector2f result{};

    result.x = point.x / tex_size.x;
    result.y = -(point.y - tex_size.y) / tex_size.y;

    return result;
}

Image::Image(const Texture& texture) : texture{texture}, m_position{}, m_size{}
{
}

const Texture& Image::getTexture() const noexcept
{
    return texture;
}

void Image::setPosition(const Vector2f position) noexcept
{
    m_position = position;
}

Vector2f Image::getPosition() const noexcept
{
    return m_position;
}

void Image::setSize(const Vector2f size) noexcept
{
    m_size = size;
}

Vector2f Image::getSize() const noexcept
{
    return m_size;
}
