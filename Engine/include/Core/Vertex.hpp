#pragma once

#include "Vector.hpp"
#include "Color.hpp"

namespace eg
{

    struct Vertex2D
    {
        Vector2f position;
        Color    color;
        Vector2f tex_coord;
    };

} // namespace eg