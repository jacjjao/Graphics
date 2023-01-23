#pragma once

#include <array>
#include <string_view>

#include "Vector.hpp"
#include "Matrix.hpp"
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
    static void initialize(unsigned font_size, unsigned screen_width, unsigned screen_height) noexcept;

    static void renderText(std::string_view text,
                           float            x,
                           float            y,
                           Color            color = Color::White,
                           unsigned         font_size  = text_size) noexcept;

    static void releaseResources() noexcept;

private:
    static std::array<Character, 128> characters;

    static uint32_t VAO, VBO;

    static Vector2f half_scr_size;

    static unsigned text_size;
};