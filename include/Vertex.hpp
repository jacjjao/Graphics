#pragma once

#include <cstdint>

#include "Vector.hpp"

class Color
{
public:
    constexpr Color() = default;

    explicit constexpr Color(uint32_t value) noexcept;
    explicit constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept;

    constexpr Color& operator=(uint32_t value) noexcept;

    static const Color Red;
    static const Color Blue;
    static const Color Green;
    static const Color Black;
    static const Color White;

    uint8_t r, g, b, a;
};

struct Vertex2D
{
    Vector2<float> position;
    Color          color;
};