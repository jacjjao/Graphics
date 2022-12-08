#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"

class Utility
{
public:
    static void initialize(float width, float height) noexcept;

    static Vector3f pointToOpenGL(Vector3f point) noexcept;
    static Vector3f vectorToOpenGL(Vector3f vector) noexcept;
    static Vector2f pointToTexCoord(Vector2f point, Vector2f tex_size) noexcept;

    static float getWindowWidth() noexcept;
    static float getWindowHeight() noexcept;
    static float getHalfWindowWidth() noexcept;
    static float getHalfWindowHeight() noexcept;
    static float radians(float degrees) noexcept;

private:
    // static Vector3f pointToGL(Vector3f point, float half_width, float half_height) noexcept;
    // static Vector3f vectorToGL(Vector3f vector, float half_width, float half_height) noexcept;

    static float window_width, window_height;
    static float half_window_width, half_window_height;
};