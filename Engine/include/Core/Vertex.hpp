#pragma once

#include "Vector.hpp"
#include "Color.hpp"

namespace Engine
{

    struct Vertex2D
    {
        Vector2f position;
        Color    color;
        Vector2f tex_coord;
    };

} // namespace Engine