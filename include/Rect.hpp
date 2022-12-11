#pragma once

#include "Vector.hpp"

class Rect
{
public:
    explicit constexpr Rect() noexcept = default;

    Vector2f position;
    Vector2f size;
};