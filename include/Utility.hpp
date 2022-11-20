#pragma once

#include "Vector.hpp"

class Utility
{
public:
    static void           initialize(float width, float height) noexcept;
    static Vector2<float> pointToOpenGL(Vector2<float> point) noexcept;
    static Vector2<float> vectorToOpenGL(Vector2<float> vector) noexcept;
    static float          getWindowWidth() noexcept;
    static float          getWindowHeight() noexcept;
    static float          getHalfWindowWidth() noexcept;
    static float          getHalfWindowHeight() noexcept;
    static float          radians(float degrees) noexcept;

private:
    static Vector2<float> pointToGL(Vector2<float> point, float half_width, float half_height) noexcept;
    static Vector2<float> vectorToGL(Vector2<float> vector, float half_width, float half_height) noexcept;

    static float window_width, window_height;
    static float half_window_width, half_window_height;
};