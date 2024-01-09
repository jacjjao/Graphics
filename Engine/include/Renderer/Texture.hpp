#pragma once

#include "include/Core/Vector.hpp"

#include <filesystem>
#include <cstdint>
#include <cstdint>
#include <string>

namespace eg
{
    class Texture
    {
    public:
        struct Params
        {
        public:
            enum class Wrapping : uint32_t
            {
                Repeat = 0x2901,
                MirroredRepeat = 0x8370,
                ClampToEdge = 0x812F,
                ClampToBorder = 0x812D
            };

            enum class Filtering : uint32_t
            {
                Nearest = 0x2600,
                Linear = 0x2601
            };

            enum class Format : uint32_t
            {
                DepthComponent = 0x1902,
                DepthStencil = 0x84F9,
                Red = 0x1903,
                RG = 0x8227,
                RGB = 0x1907,
                RGBA = 0x1908,
            };

            Wrapping  wrap_s = Wrapping::Repeat, wrap_t = Wrapping::Repeat;
            Filtering min_filter = Filtering::Linear, mag_filter = Filtering::Linear;
            Format format = Format::RGBA;
            bool use_mipmap = false;
        };

        Texture() = default;
        explicit Texture(const std::filesystem::path& path, const Params& parameters = {});
        explicit Texture(const void* data, int32_t width, int32_t height, const Params& parameters = {});
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void destroy();

        [[nodiscard]] 
        Vector2f getSize() const { return m_size; }

        [[nodiscard]] 
        float getWidth() const { return m_size.x; }

        [[nodiscard]] 
        float getHeight() const { return m_size.y; }

        static void bind(Texture* texture);
        static void unbind();

        void createFromImage(const std::filesystem::path& path, const Params& parameters = {});
        void createFromData(const void* data, int32_t width, int32_t height, const Params& parameters = {});

    private:
        static uint32_t tex_in_bind;

        uint32_t m_id;

        Vector2f m_size;
    };

}