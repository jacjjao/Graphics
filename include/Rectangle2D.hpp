#pragma once

#include <cstdint>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"

class Rectangle2D
{
public:
    explicit Rectangle2D(glm::vec2 position, uint32_t width, uint32_t height);

    Rectangle2D(Rectangle2D&) = delete;
    Rectangle2D& operator=(Rectangle2D&) = delete;

    void draw();
    void update();
    void setPosition(glm::vec2 position) noexcept;
    void setColor(Color color) noexcept;
    void setWidth(uint32_t width) noexcept;
    void setHeight(uint32_t height) noexcept;

    [[nodiscard]] glm::vec2 getPosition() const noexcept;

private:
    void create();

    glm::vec2 m_position;
    uint32_t m_width, m_height;
    Color m_color;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};