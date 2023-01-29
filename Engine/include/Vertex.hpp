#pragma once

#include "Vector.hpp"
#include "Color.hpp"

struct Vertex
{
    Vertex() = default;

    Vector3f position;
    Color    color;
    Vector2f tex_coord;
};