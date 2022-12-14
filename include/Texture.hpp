#pragma once

#include "Vector.hpp"

#include <filesystem>
#include <cstdint>
#include <stdint.h>
#include <string>

namespace Parameter
{
enum class Wrapping : uint32_t
{
    Repeat         = 0x2901,
    MirroredRepeat = 0x8370,
    ClampToEdge    = 0x812F,
    ClampToBorder  = 0x812D
};

enum class Filtering : uint32_t
{
    Nearest = 0x2600,
    Linear  = 0x2601
};

enum class TexFormat : uint32_t
{
    DepthComponent = 0x1902,
    DepthStencil   = 0x84F9,
    Red            = 0x1903,
    RG             = 0x8227,
    RGB            = 0x1907,
    RGBA           = 0x1908,
};
}; // namespace Parameter

class TexConstructParameter
{
public:
    TexConstructParameter() noexcept;

    Parameter::Wrapping  warp_s, warp_t;
    Parameter::Filtering min_filter, mag_filter;
    Parameter::TexFormat format;
    bool                 useMipmap;
};

class Texture
{
public:
    explicit Texture(const std::filesystem::path& path, TexConstructParameter parameters = {}) noexcept;
    ~Texture() noexcept;

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept            = default;
    Texture& operator=(Texture&&) noexcept = default;

    void destroy() noexcept;

    [[nodiscard]] Vector2f getSize() const noexcept;

    [[nodiscard]] float getWidth() const noexcept;
    [[nodiscard]] float getHeight() const noexcept;

    static void bind(Texture*) noexcept;
    static void unbind() noexcept;

    static Vector2f pointToTexCoord(Vector2f point, Vector2f tex_size) noexcept;

private:
    static Texture* texture_in_bind;

    uint32_t m_id;
    Vector2f m_size;
};