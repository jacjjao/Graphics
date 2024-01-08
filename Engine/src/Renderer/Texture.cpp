#include "pch.hpp"
#include "include/Renderer/Texture.hpp"
#include "include/Renderer/ShaderProgram.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace eg
{
    uint32_t Texture::tex_in_bind = 0;

    Texture::Texture(const std::filesystem::path& path, const Params& parameters) : m_id{ 0 }, m_size{}
    {
        createFromImage(path);
    }

    Texture::Texture(const void* data, int32_t width, int32_t height, const Params& parameters) :
        m_id{ 0 },
        m_size{}
    {
        createFromData(data, width, height, parameters);
    }

    Texture::~Texture()
    {
        destroy();
    }

    void Texture::createFromImage(const std::filesystem::path& path, const Params& parameters)
    {
        // load image
        int width = 0;
        int height = 0;
        int nr_channels = 0;
        stbi_set_flip_vertically_on_load(true);

        stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &nr_channels, STBI_rgb_alpha);
        if (data != nullptr)
        {
            createFromData(data, width, height, parameters);
        }
        else
        {
            std::cerr << "Failed to load texture: " << path << '\n';
        }

        stbi_image_free(data);
    }

    void Texture::createFromData(const void* data, const int32_t width, const int32_t height, const Params& parameters)
    {
        // create texture
        glGenTextures(1, &m_id);
        bind(this);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(parameters.wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(parameters.wrap_t));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(parameters.min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(parameters.mag_filter));

        m_size.x = static_cast<float>(width);
        m_size.y = static_cast<float>(height);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            static_cast<GLenum>(parameters.format),
            width,
            height,
            0,
            static_cast<GLenum>(parameters.format),
            GL_UNSIGNED_BYTE,
            data
        );

        if (parameters.use_mipmap)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        unbind();
    }

    Texture::Texture(Texture&& other) noexcept :
        m_id{ other.m_id }, 
        m_size{ other.m_size }
    {
        other.m_id = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept
    {
        destroy();

        m_id   = other.m_id;
        m_size = other.m_size;

        other.m_id = 0;

        return *this;
    }

    void Texture::destroy()
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

    void Texture::bind(Texture* texture, const size_t unit_index)
    {
        if (tex_in_bind != texture->m_id)
        {
            glBindTexture(GL_TEXTURE_2D, texture->m_id);
            tex_in_bind = texture->m_id;
        }
    }

    void Texture::unbind(const size_t unit_index)
    {
        if (tex_in_bind != 0)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            tex_in_bind = 0;
        }
    }

}