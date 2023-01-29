#include "../include/pch.hpp"
#include "../include/Texture.hpp"
#include "../include/ShaderProgram.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

TexConstructParams::TexConstructParams() :
wrap_s{Wrapping::Repeat},
wrap_t{Wrapping::Repeat},
min_filter{Filtering::Linear},
mag_filter{Filtering::Linear},
format{Format::RGB},
use_mipmap{false}
{
}

std::array<Texture*, 32> Texture::textures_in_bind{};

void Texture::Init()
{
    auto& program = DefaultShaderProgram::instance();
    program.use();
    
    char tex0_9[] = "textures[0]";
    for (int i = 0; i < 10; i++)
    {
        tex0_9[9] = i + '0';
        program.setI32(tex0_9, i);
    }
    char tex10_31[] = "textures[10]";
    for (int i = 10; i < 32; i++)
    {
        tex10_31[9] = (i / 10) + '0';
        tex10_31[10] = (i % 10) + '0';
        program.setI32(tex10_31, i);
    }

    program.unuse();
}

Texture::Texture(const std::filesystem::path& path, const TexConstructParams& parameters) : m_id{0}, m_size{}
{
    createFromImage(path);
}

Texture::Texture(const void* data, int32_t width, int32_t height, const TexConstructParams& parameters) :
m_id{0},
m_size{}
{
    createFromData(data, width, height, parameters);
}

Texture::~Texture()
{
    destroy();
}

void Texture::createFromImage(const std::filesystem::path& path, const TexConstructParams& parameters)
{
    // load image
    int width      = 0;
    int height     = 0;
    int nr_channels = 0;
    stbi_set_flip_vertically_on_load(true);

    auto data = stbi_load(path.string().c_str(), &width, &height, &nr_channels, STBI_rgb);
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

void Texture::createFromData(const void* data, const int32_t width, const int32_t height, const TexConstructParams& parameters)
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

Texture::Texture(Texture&& other) : m_id{other.m_id}, m_size{other.m_size}
{
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other)
{
    m_id = other.m_id;
    m_size = other.m_size;

    other.m_id = 0;

    return *this;
}

void Texture::destroy()
{
    if (m_id != 0)
    {
        glCheck(glDeleteTextures(1, &m_id));
        m_id = 0;
    }
}

Vector2f Texture::getSize() const
{
    return m_size;
}

float Texture::getWidth() const
{
    return m_size.x;
}

float Texture::getHeight() const
{
    return m_size.y;
}

size_t Texture::getUnit() const
{
    return which_unit;
}

void Texture::bind(Texture* texture, const size_t unit_index)
{
    if (textures_in_bind[unit_index] != texture)
    {
        texture->which_unit = unit_index;
        glCheck(glActiveTexture(GL_TEXTURE0 + unit_index));
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->m_id));
        textures_in_bind[unit_index] = texture;
    }
}

void Texture::unbind(const size_t unit_index)
{
    if (textures_in_bind[unit_index] != nullptr)
    {
        glCheck(glActiveTexture(GL_TEXTURE0 + unit_index));
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
        textures_in_bind[unit_index] = nullptr;
    }
}
