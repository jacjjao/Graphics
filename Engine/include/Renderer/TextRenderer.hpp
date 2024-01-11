#pragma once

#include <array>

#include "include/Core/Vector.hpp"
#include "include/Core/Matrix.hpp"
#include "include/Core/Color.hpp"
#include "include/Renderer/Texture.hpp"
#include "include/Renderer/VertexArray.hpp"

namespace eg
{

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
        static void init(unsigned font_size);

        static void renderText(const std::string& text,
                               Vector2f pos,
                               Color    color = Color::White,
                               unsigned font_size = s_text_size);

    private:
        static std::array<Character, 128> s_characters;

        static VertexArray s_vao;

        static unsigned s_text_size;
    };

} // namespace eg