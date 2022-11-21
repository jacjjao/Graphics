#pragma once

#include "Vector.hpp"

class Utility
{
public:
    static void     initialize(float width, float height) noexcept;
    static Vector2f pointToOpenGL(Vector2f point) noexcept;
    static Vector2f vectorToOpenGL(Vector2f vector) noexcept;
    static float    getWindowWidth() noexcept;
    static float    getWindowHeight() noexcept;
    static float    getHalfWindowWidth() noexcept;
    static float    getHalfWindowHeight() noexcept;
    static float    radians(float degrees) noexcept;

private:
    static Vector2f pointToGL(Vector2f point, float half_width, float half_height) noexcept;
    static Vector2f vectorToGL(Vector2f vector, float half_width, float half_height) noexcept;

    static float window_width, window_height;
    static float half_window_width, half_window_height;
};