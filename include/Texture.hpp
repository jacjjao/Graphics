#pragma once

#include "Vector.hpp"

#include <filesystem>
#include <cstdint>
#include <stdint.h>
#include <string>


class TexConstructParams
{
public:
    TexConstructParams() noexcept;

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

    enum class Format : uint32_t
    {
        DepthComponent = 0x1902,
        DepthStencil   = 0x84F9,
        Red            = 0x1903,
        RG             = 0x8227,
        RGB            = 0x1907,
        RGBA           = 0x1908,
    };

    Wrapping  wrap_s, wrap_t;
    Filtering min_filter, mag_filter;
    Format format;
    bool use_mipmap;
};

class Texture
{
public:
    static void Init() noexcept;

    Texture() = default;
    explicit Texture(const std::filesystem::path& path, const TexConstructParams& parameters = {}) noexcept;
    explicit Texture(const void* data, int32_t width, int32_t height, const TexConstructParams& parameters = {}) noexcept;
    ~Texture() noexcept;

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void destroy() noexcept;

    [[nodiscard]] Vector2f getSize() const noexcept;

    [[nodiscard]] float getWidth() const noexcept;
    [[nodiscard]] float getHeight() const noexcept;

    [[nodiscard]] size_t getUnit() const noexcept;

    static void bind(Texture* texture, size_t unit_index = 0) noexcept;
    static void unbind(size_t unit_index = 0) noexcept;

    void createFromImage(const std::filesystem::path& path, const TexConstructParams& parameters = {}) noexcept;
    void createFromData(const void* data, int32_t width, int32_t height, const TexConstructParams& parameters = {}) noexcept;

private:
    static std::array<Texture*, 32> textures_in_bind;

    uint32_t m_id;

    size_t which_unit = 0;

    Vector2f m_size;
};