#include "../include/pch.hpp"
#include "../include/TextRenderer.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Math.hpp"

#include <iostream>

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

std::array<Character, 128> TextRenderer::characters;

uint32_t TextRenderer::VAO = 0, TextRenderer::VBO = 0;

unsigned TextRenderer::text_size = 0;
Vector2f TextRenderer::half_scr_size{};

void TextRenderer::initialize(const unsigned font_size, const unsigned screen_width, const unsigned screen_height) noexcept
{
    text_size = font_size;
    half_scr_size.x = static_cast<float>(screen_width / 2);
    half_scr_size.y = static_cast<float>(screen_height / 2);

    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    const auto font_name = FileSystem::getPath("/asset/fonts/arial.ttf").string();
    if (font_name.empty())
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, font_size);

    // disable byte-alignment restriction
    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    TexConstructParameter parameters{};
    parameters.wrap_s     = parameters.wrap_t     = Parameter::Wrapping::ClampToEdge;
    parameters.min_filter = parameters.mag_filter = Parameter::Filtering::Linear;
    parameters.format                             = Parameter::Format::Red;

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << (int)c << '\n';
            continue;
        }

        // generate texture
        Texture texture{face->glyph->bitmap.buffer,
                        static_cast<int32_t>(face->glyph->bitmap.width),
                        static_cast<int32_t>(face->glyph->bitmap.rows),
                        parameters};

        // now store character for later use
        characters[c] = Character{std::move(texture),
                                  Vector2f(static_cast<float>(face->glyph->bitmap.width),
                                           static_cast<float>(face->glyph->bitmap.rows)),
                                  Vector2f(static_cast<float>(face->glyph->bitmap_left),
                                           static_cast<float>(face->glyph->bitmap_top)),
                                  static_cast<unsigned>(face->glyph->advance.x)};
    }

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glCheck(glGenVertexArrays(1, &VAO));
    glCheck(glGenBuffers(1, &VBO));
    glCheck(glBindVertexArray(VAO));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW));
    glCheck(glEnableVertexAttribArray(0));
    glCheck(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    glCheck(glBindVertexArray(0));
}

void TextRenderer::renderText(std::string_view text, Vector2f pos, const Color color, const unsigned font_size) noexcept
{
    static auto proj = ortho(-half_scr_size.x, half_scr_size.x, -half_scr_size.y, half_scr_size.y, -1.0, 1.0);

    glCheck(glEnable(GL_BLEND));

    const float scale = static_cast<float>(font_size) / static_cast<float>(text_size);

    pos.y -= static_cast<float>(font_size);

    auto& shader = TextShaderProgram::instance();
    shader.use();

    shader.setVec4("textColor", color);
    shader.setMat4("proj", proj);

    // iterate through all characters
    for (const char& c : text)
    {
        auto& ch = characters[c];

        const float xpos = pos.x + ch.bearing.x * scale;
        const float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

        const float w = ch.size.x * scale;
        const float h = ch.size.y * scale;
        // update VBO for each character

        // clang-format off
        const float vertices[24] = {xpos,     ypos + h, 0.0f, 0.0f,           
                                    xpos,     ypos,     0.0f, 1.0f,           
                                    xpos + w, ypos,     1.0f, 1.0f,

                                    xpos,     ypos + h, 0.0f, 0.0f,           
                                    xpos + w, ypos,     1.0f, 1.0f,           
                                    xpos + w, ypos + h, 1.0f, 0.0f};
        // clang-format on

        // render glyph texture over quad
        Texture::bind(&ch.texture);
        // update content of VBO memory
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // render quad
        glCheck(glBindVertexArray(VAO));
        glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        pos.x += (ch.advance >> 6) * scale;
    }
    glCheck(glBindVertexArray(0));
    Texture::unbind();

    glCheck(glDisable(GL_BLEND));
}

void TextRenderer::releaseResources() noexcept
{
    for (auto& character : characters)
    {
        character.texture.destroy();
    }
}
