#pragma once

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <vector>
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"

class Rectangle2D
{
public:
    explicit Rectangle2D(glm::ivec2 position, uint32_t width, uint32_t height);

    Rectangle2D(Rectangle2D&) = delete;
    Rectangle2D& operator=(Rectangle2D&) = delete;

    void draw();
    void update();
    void set_position(glm::ivec2 position);
    void set_color(Color color);
    void set_width(uint32_t width);
    void set_height(uint32_t height);

    [[nodiscard]] glm::ivec2 get_pos() const noexcept;

private:
    void update_vbo_data();
    void create();

    glm::ivec2 m_pos;
    uint32_t m_width, m_height;
    Color m_color;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};