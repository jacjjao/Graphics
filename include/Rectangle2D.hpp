#pragma once

#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"

class Rectangle2D
{
public:
    explicit Rectangle2D(float width, float height);

    Rectangle2D(Rectangle2D&) = delete;
    Rectangle2D& operator=(Rectangle2D&) = delete;

    void draw();
    void update();

    void setPosition(glm::vec2 position) noexcept;
    void setColor(Color color) noexcept;
    void setWidth(float width) noexcept;
    void setHeight(float height) noexcept;

    void translate(glm::vec2 vector) noexcept;
    void scale(glm::vec2 factor) noexcept;
    void rotate(float degree) noexcept;

    [[nodiscard]] glm::vec2 getPosition() const noexcept;

private:
    void create();

    glm::vec2 m_position;
    float m_width, m_height;
    Color m_color;

    glm::mat4 m_model;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};