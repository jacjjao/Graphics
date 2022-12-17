#pragma once

#include "Vector.hpp"
#include "Color.hpp"

class Vertex
{
public:
    explicit constexpr Vertex() noexcept : position{}, color{}, tex_coord{}
    {
    }

    Vector3f position;
    Color    color;
    Vector2f tex_coord;
};