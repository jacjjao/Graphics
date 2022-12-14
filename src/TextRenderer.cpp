#include "../include/TextRenderer.hpp"
#include "../include/FileSystem.hpp"
#include "../include/glCheck.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Window.hpp"

#include <iostream>

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

std::array<Character, 128> TextRenderer::Characters;

unsigned TextRenderer::font_size_on_load = default_font_size;

void TextRenderer::initialize(const unsigned font_size) noexcept
{
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    std::string font_name = FileSystem::getPath("/asset/fonts/BitterPro-Light.ttf").string();
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

        unsigned int texture;
        glCheck(glGenTextures(1, &texture));
        glCheck(glBindTexture(GL_TEXTURE_2D, texture));
        glCheck(glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RED,
                             face->glyph->bitmap.width,
                             face->glyph->bitmap.rows,
                             0,
                             GL_RED,
                             GL_UNSIGNED_BYTE,
                             face->glyph->bitmap.buffer));
        // set texture options
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        // now store character for later use
        Character character = {texture,
                               Vector2f(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               Vector2f(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               static_cast<unsigned int>(face->glyph->advance.x)};
        Characters[c]       = character;
    }
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(const std::string& text, float x, float y, const Color color, const unsigned size) noexcept
{
    glCheck(glEnable(GL_BLEND));

    const float scale = static_cast<float>(size) / static_cast<float>(font_size_on_load);

    y += static_cast<float>(size);

    auto& shader = TextShaderProgram::instance();
    shader.use();

    shader.setVec4("textColor", color);

    static GLuint VAO = 0;
    static GLuint VBO = 0;
    if (VAO == 0)
    {
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

    // iterate through all characters
    for (const char& c : text)
    {
        const auto& ch = Characters[c];

        const float xpos = x + ch.Bearing.x * scale;
        const float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        /* const float xpos = x - ch.Bearing.x * scale;
        const float ypos = y + ch.Bearing.y * scale; */

        const float w = ch.Size.x * scale;
        const float h = ch.Size.y * scale;
        // update VBO for each character
        const auto pos  = Window::pointToOpenGL(Vector3f{xpos, ypos, 0.0F});
        const auto size = Window::vectorToOpenGL(Vector3f{w, -h, 0.0F});

        // clang-format off
        const float vertices[24] = {pos.x,          pos.y + size.y, 0.0f,  0.0f,           
                                    pos.x,          pos.y,          0.0f,  1.0f,           
                                    pos.x + size.x, pos.y,         1.0f, 1.0f,

                                    pos.x,          pos.y + size.y, 0.0f, 0.0f,           
                                    pos.x + size.x, pos.y,          1.0f, 1.0f,           
                                    pos.x + size.x, pos.y + size.y, 1.0f, 0.0f};
        // clang-format on

        // render glyph texture over quad
        glCheck(glBindTexture(GL_TEXTURE_2D, ch.TextureID));
        // update content of VBO memory
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        glCheck(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices)); // be sure to use glBufferSubData and not glBufferData

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // render quad
        glCheck(glBindVertexArray(VAO));
        glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        x += (ch.Advance >> 6) * scale;
    }
    glCheck(glBindVertexArray(0));
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));

    glCheck(glDisable(GL_BLEND));
}