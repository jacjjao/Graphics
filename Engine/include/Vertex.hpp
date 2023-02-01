#pragma once

#include "Vector.hpp"
#include "Color.hpp"

namespace Engine
{

    struct Vertex
    {
        Vector3f position;
        Color    color;
        Vector2f tex_coord;
    };

} // namespace Engine