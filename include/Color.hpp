#pragma once

#include <cstdint>

class Color
{
public:
    constexpr Color() noexcept = default;

    explicit constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept :
    r(red),
    g(green),
    b(blue),
    a(alpha)
    {
    }

    static constexpr Color fromHex(uint32_t hex) noexcept 
    {
        const auto r = static_cast<uint8_t>((hex & 0xFF000000) >> 24);
        const auto g = static_cast<uint8_t>((hex & 0x00FF0000) >> 16);
        const auto b = static_cast<uint8_t>((hex & 0x0000FF00) >> 8);
        const auto a = static_cast<uint8_t>((hex & 0x000000FF));

        return Color{r, g, b, a};
    }

    static const Color Red;
    static const Color Cyan;
    static const Color Blue;
    static const Color Green;
    static const Color Black;
    static const Color White;

    uint8_t r = 0, g = 0, b = 0, a = 0;
};
