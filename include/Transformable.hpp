#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>

class Transformable
{
public:
    Transformable();
    virtual ~Transformable() = default;

    virtual void translate(glm::vec2 vector) = 0;
    virtual void scale(glm::vec2 factor) = 0;
    virtual void rotate(float degree) = 0;

protected:
    glm::mat4 m_model;
};