#include "pch.hpp"
#include "include/Renderer/TextRenderer.hpp"
#include "include/Renderer/ShaderProgram.hpp"
#include "include/Core/Math.hpp"
#include "include/Core/Log.hpp"
#include "include/Core/Application.hpp"
#include "include/Renderer/VertexBuffer.hpp"
#include "include/Renderer/VertexArray.hpp"

#include "src/Core/FileSystem.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace eg
{
    std::array<Character, 128> TextRenderer::s_characters;

    unsigned TextRenderer::s_text_size = 0;

    void TextRenderer::init(const unsigned font_size)
    {
        s_text_size = font_size;
        const auto half_width = static_cast<float>(Application::getInstance().getWindow().getWidth()) * 0.5f;
        const auto half_height = static_cast<float>(Application::getInstance().getWindow().getHeight()) * 0.5f;

        auto& shader = TextShaderProgram::instance();
        shader.use();
        shader.setMat4("proj", ortho(-half_width, half_width, -half_height, half_height, -1.0f, 1.0f));
        shader.unuse();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            EG_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        const auto font_name = FileSystem::getPath("/asset/fonts/arial.ttf").string();
        if (font_name.empty())
        {
            EG_CORE_ERROR("ERROR::FREETYPE: Failed to load font_name");
            return;
        }

        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face))
        {
            EG_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
            return;
        }

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, font_size);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        Texture::Params parameters{};
        parameters.wrap_s = parameters.wrap_t = Texture::Params::Wrapping::ClampToEdge;
        parameters.min_filter = parameters.mag_filter = Texture::Params::Filtering::Linear;
        parameters.format = Texture::Params::Format::Red;

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                EG_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph: {}", (int)c);
                continue;
            }

            // generate texture
            Texture texture{ face->glyph->bitmap.buffer,
                            static_cast<int32_t>(face->glyph->bitmap.width),
                            static_cast<int32_t>(face->glyph->bitmap.rows),
                            parameters };

            // now store character for later use
            s_characters[c] = Character{ std::move(texture),
                                      Vector2f(static_cast<float>(face->glyph->bitmap.width),
                                               static_cast<float>(face->glyph->bitmap.rows)),
                                      Vector2f(static_cast<float>(face->glyph->bitmap_left),
                                               static_cast<float>(face->glyph->bitmap_top)),
                                      static_cast<unsigned>(face->glyph->advance.x) };
        }

        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        s_vao.emplace();
        s_vbo.emplace(sizeof(float) * 6 * 4, VertexBufferUsage::DynamicDraw);
        constexpr BufferLayout layout{
            .index = 0, .component_count = 4, .type = GL_FLOAT, .normalize = false, .stride = 4 * sizeof(float), .offset = 0
        };
        s_vao->addVertexBuffer(*s_vbo, {std::addressof(layout), 1});
    }

    void TextRenderer::renderText(const std::string& text, Vector2f pos, const Color color, const unsigned font_size)
    {
        glEnable(GL_BLEND);

        const float scale = static_cast<float>(font_size) / static_cast<float>(s_text_size);

        pos.y -= static_cast<float>(font_size);

        auto& shader = TextShaderProgram::instance();
        shader.use();
        shader.setVec4("textColor", color);

        // iterate through all characters
        for (const char& c : text)
        {
            auto& ch = s_characters[c];

            const float xpos = pos.x + ch.bearing.x * scale;
            const float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

            const float w = ch.size.x * scale;
            const float h = ch.size.y * scale;
            // update VBO for each character

            // clang-format off
            const float vertices[24] = { xpos,     ypos + h, 0.0f, 0.0f,
                                         xpos,     ypos,     0.0f, 1.0f,
                                         xpos + w, ypos,     1.0f, 1.0f,

                                         xpos,     ypos + h, 0.0f, 0.0f,
                                         xpos + w, ypos,     1.0f, 1.0f,
                                         xpos + w, ypos + h, 1.0f, 0.0f };
            // clang-format on

            // render glyph texture over quad
            Texture::bind(&ch.texture);
            // update content of VBO memory
            
            std::ranges::copy(vertices, s_vbo->begin());
            s_vbo->update();

            VertexArray::bind(*s_vao);

            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            pos.x += (ch.advance >> 6) * scale;
        }
        VertexArray::unbind();
        Texture::unbind();

        glDisable(GL_BLEND);
    }

} // namespace eg