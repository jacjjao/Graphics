#include "../include/Texture.hpp"
#include "../include/glCheck.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>

TexConstructParameter::TexConstructParameter() noexcept :
wrap_s{Parameter::Wrapping::Repeat},
wrap_t{Parameter::Wrapping::Repeat},
min_filter{Parameter::Filtering::Linear},
mag_filter{Parameter::Filtering::Linear},
format{Parameter::Format::RGB},
use_mipmap{false}
{
}

Texture* Texture::texture_in_bind = nullptr;

Texture::Texture(const std::filesystem::path& path, TexConstructParameter parameters) noexcept : m_id{0}, m_size{}
{
    createFromImage(path);
}

Texture::Texture(const void* data, int32_t width, int32_t height, TexConstructParameter parameters) noexcept :
m_id{0},
m_size{}
{
    createFromData(data, width, height, parameters);
}

Texture::~Texture() noexcept
{
    destroy();
}

void Texture::createFromImage(const std::filesystem::path& path, const TexConstructParameter parameters) noexcept
{
    // load image
    int width      = 0;
    int height     = 0;
    int nr_channels = 0;
    stbi_set_flip_vertically_on_load(true);

    uint8_t* data = stbi_load(path.string().c_str(), &width, &height, &nr_channels, 0);
    if (data != nullptr)
    {
        createFromData(data, width, height, parameters);
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << '\n';
    }

    stbi_image_free(data);
    unbind();
}

void Texture::createFromData(const void* data, const int32_t width, const int32_t height, const TexConstructParameter parameters) noexcept
{
    // create texture
    glCheck(glGenTextures(1, &m_id));
    bind(this);

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(parameters.wrap_s)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(parameters.wrap_t)));

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(parameters.min_filter)));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(parameters.mag_filter)));

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

    if (parameters.use_mipmap)
    {
        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    }

    unbind();
}

Texture::Texture(Texture&& other) noexcept : m_id{other.m_id}, m_size{other.m_size}
{
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_id = other.m_id;
    m_size = other.m_size;

    other.m_id = 0;

    return *this;
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

Vector2f Texture::pointToTexCoord(const Vector2f point) const noexcept
{
    Vector2f result{};

    result.x = point.x / m_size.x;
    result.y = -(point.y - m_size.y) / m_size.y;

    return result;
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
