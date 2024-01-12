#pragma once

#include "Core.hpp"
#include "Vector.hpp"
#include "Color.hpp"

namespace eg
{

    struct EG_API Vertex
    {
        Vector3f position;
        Color    color;
        Vector2f tex_coord;
    };

    static_assert(sizeof(Vertex) == sizeof(Vector3f) + sizeof(Color) + sizeof(Vector2f));

} // namespace eg