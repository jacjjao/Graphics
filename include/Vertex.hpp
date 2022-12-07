#pragma once

#include <cstdint>

#include "Vector.hpp"
#include "Color.hpp"

class Vertex2D
{
public:
    explicit constexpr Vertex2D() noexcept : position{}, color{}, tex_coord{}
    {
    }

    Vector2f position;
    Color    color;
    Vector2f tex_coord;
};