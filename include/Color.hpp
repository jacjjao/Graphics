#pragma once

#include <cstdint>

class Color
{
public:
    constexpr Color() noexcept = default;

    explicit constexpr Color(uint32_t value) noexcept :
    r(static_cast<uint8_t>((value & 0xFF000000) >> 24)),
    g(static_cast<uint8_t>((value & 0x00FF0000) >> 16)),
    b(static_cast<uint8_t>((value & 0x0000FF00) >> 8)),
    a(static_cast<uint8_t>((value & 0x000000FF)))
    {
    }

    explicit constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept :
    r(red),
    g(green),
    b(blue),
    a(alpha)
    {
    }

    constexpr Color& operator=(uint32_t value) noexcept
    {
        r = static_cast<uint8_t>((value & 0xFF000000) >> 24);
        g = static_cast<uint8_t>((value & 0x00FF0000) >> 16);
        b = static_cast<uint8_t>((value & 0x0000FF00) >> 8);
        a = static_cast<uint8_t>((value & 0x000000FF));
        return *this;
    }

    static const Color Red;
    static const Color Blue;
    static const Color Green;
    static const Color Black;
    static const Color White;

    uint8_t r, g, b, a;
};
