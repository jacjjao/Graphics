#pragma once

#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include <glm/ext/vector_int2.hpp>

class Circle2D
{
public:
    explicit Circle2D(glm::vec2 position, float radius, size_t point_count = 30);

    void draw();
    void update();
    void setColor(Color color);

private:
    void create();

    glm::vec2 m_position;
    float m_radius;
    Color m_color;

    VertexArray m_vao;
    ElementBuffer m_ebo;
};