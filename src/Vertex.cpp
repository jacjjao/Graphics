#include "../include/Vertex.hpp"
#include <cstdint>

const Color Color::Red{0xFF0000FF};
const Color Color::Green{0x00FF00FF};
const Color Color::Blue{0x0000FFFF};
const Color Color::Black{0x000000FF};
const Color Color::White{0xFFFFFFFF};

constexpr Color::Color(const uint32_t value)
    : r(static_cast<uint8_t>((value & 0xFF000000) >> 24)), g(static_cast<uint8_t>((value & 0x00FF0000) >> 16)),
      b(static_cast<uint8_t>((value & 0x0000FF00) >> 8)), a(static_cast<uint8_t>((value & 0x000000FF)))
{
}

constexpr Color::Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
    : r(red), g(green), b(blue), a(alpha)
{
}

constexpr Color& Color::operator=(const uint32_t value)
{
    r = static_cast<uint8_t>((value & 0xFF000000) >> 24);
    g = static_cast<uint8_t>((value & 0x00FF0000) >> 16);
    b = static_cast<uint8_t>((value & 0x0000FF00) >> 8);
    a = static_cast<uint8_t>((value & 0x000000FF));
    return *this;
}