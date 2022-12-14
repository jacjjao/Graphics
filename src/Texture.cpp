#include "../include/Texture.hpp"
#include "../include/glCheck.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

TexConstructParameter::TexConstructParameter() noexcept :
warp_s{Parameter::Wrapping::Repeat},
warp_t{Parameter::Wrapping::Repeat},
min_filter{Parameter::Filtering::Linear},
mag_filter{Parameter::Filtering::Linear},
format{Parameter::TexFormat::RGB},
useMipmap{true}
{
}

Texture* Texture::texture_in_bind = nullptr;

Texture::Texture(const std::filesystem::path& path, const TexConstructParameter parameters) noexcept : m_id{}, m_size{}
{
    // create texture
    glCheck(glGenTextures(1, &m_id));
    bind(this);

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(parameters.warp_s)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(parameters.warp_t)));

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(parameters.min_filter)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(parameters.mag_filter)));

    // load image
    int width      = 0;
    int height     = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(true);

    uint8_t* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);

    bool load_success = (data != nullptr);
    if (load_success)
    {
        m_size.x = static_cast<float>(width);
        m_size.y = static_cast<float>(height);
        glCheck(glTexImage2D(GL_TEXTURE_2D,
                             0,
                             static_cast<GLenum>(parameters.format),
                             width,
                             height,
                             0,
                             static_cast<GLenum>(parameters.format),
                             GL_UNSIGNED_BYTE,
                             data));
        if (parameters.useMipmap)
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        }
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
