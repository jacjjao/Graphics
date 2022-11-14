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
    void setPosition(glm::ivec2 position);
    void setColor(Color color);
    void setWidth(uint32_t width);
    void setHeight(uint32_t height);

    [[nodiscard]] glm::ivec2 getPosition() const noexcept;

private:
    void updateVboData();
    void create();

    glm::ivec2 m_position;
    uint32_t m_width, m_height;
    Color m_color;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};