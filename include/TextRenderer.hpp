#pragma once

#include <array>
#include <string>

#include "Vector.hpp"
#include "Color.hpp"

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    Vector2f     Size;      // Size of glyph
    Vector2f     Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
    static void initialize(unsigned font_size = default_font_size) noexcept;

    static void RenderText(const std::string& text,
                           float              x,
                           float              y,
                           Color              color = Color::White,
                           unsigned           size  = default_font_size) noexcept;

private:
    static constexpr unsigned default_font_size = 96;

    static std::array<Character, 128> Characters;

    static unsigned font_size_on_load;
};