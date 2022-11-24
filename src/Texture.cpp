#include "../include/Texture.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/glCheck.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

Texture::Texture(const std::string& path) noexcept : m_id{}, m_size{}
{
    // create texture
    glCheck(glGenTextures(1, &m_id));
    bind(*this);

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

    unbind();
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

float Texture::getWidth() const noexcept
{
    return m_size.x;
}

float Texture::getHeight() const noexcept
{
    return m_size.y;
}

void Texture::bind(const Texture& texture) noexcept
{
    glCheck(glBindTexture(GL_TEXTURE_2D, texture.m_id));
}

void Texture::unbind() noexcept
{
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}