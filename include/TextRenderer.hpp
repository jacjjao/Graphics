#pragma once

#include <array>
#include <string>

#include "Vector.hpp"
#include "Color.hpp"
#include "Texture.hpp"

struct Character
{
    Texture      texture; // ID handle of the glyph texture
    Vector2f     size;      // size of glyph
    Vector2f     bearing;   // Offset from baseline to left/top of glyph
    unsigned int advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
    static void initialize() noexcept;

    static void renderText(const std::string& text,
                           float              x,
                           float              y,
                           Color              color = Color::White,
                           unsigned           size  = default_font_size) noexcept;

private:
    static constexpr unsigned default_font_size = 96;

    static std::array<Character, 128> characters;

    static uint32_t VAO, VBO;
};