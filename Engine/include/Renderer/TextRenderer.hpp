#pragma once

#include <array>

#include "include/Core/Core.hpp"
#include "include/Core/Vector.hpp"
#include "include/Core/Matrix.hpp"
#include "include/Core/Color.hpp"
#include "include/Renderer/Texture.hpp"
#include "include/Renderer/VertexArray.hpp"
#include "include/Renderer/VertexBuffer.hpp"

namespace eg
{

    struct Character
    {
        Texture      texture; // ID handle of the glyph texture
        Vector2f     size;      // size of glyph
        Vector2f     bearing;   // Offset from baseline to left/top of glyph
        unsigned int advance;   // Horizontal offset to advance to next glyph
    };

    class EG_API TextRenderer
    {
    public:
        static void init(unsigned font_size);

        static void drawText(const std::string& text,
                             float x, float y,
                             Color color = Color::White,
                             unsigned font_size = s_text_size);

    private:
        static std::array<Character, 128> s_characters;

        static constexpr size_t s_max_char_count = 100;
        static constexpr size_t s_char_vertex_count = 6 * 4;

        static inline std::optional<VertexArray> s_vao = std::nullopt;
        static inline std::optional<VertexBuffer<float>> s_vbo = std::nullopt;

        static unsigned s_text_size;
    };

} // namespace eg