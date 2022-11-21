#include "../include/Utility.hpp"

float Utility::window_width       = 0.0F;
float Utility::window_height      = 0.0F;
float Utility::half_window_width  = 0.0F;
float Utility::half_window_height = 0.0F;

void Utility::initialize(const float width, const float height) noexcept
{
    window_width       = width;
    window_height      = height;
    half_window_width  = width / 2.0F;
    half_window_height = height / 2.0F;
}

Vector2<float> Utility::pointToOpenGL(const Vector2<float> point) noexcept
{
    return pointToGL(point, half_window_width, half_window_height);
}

Vector2<float> Utility::vectorToOpenGL(const Vector2<float> vector) noexcept
{
    return vectorToGL(vector, half_window_width, half_window_height);
}

float Utility::getWindowWidth() noexcept
{
    return window_width;
}

float Utility::getWindowHeight() noexcept
{
    return window_height;
}

float Utility::getHalfWindowWidth() noexcept
{
    return half_window_width;
}

float Utility::getHalfWindowHeight() noexcept
{
    return half_window_height;
}

float Utility::radians(const float degrees) noexcept
{
    return degrees * static_cast<float>(0.01745329251994329576923690768489); // copy from glm::radians
}

Vector2<float> Utility::pointToGL(const Vector2<float> point, const float half_width, const float half_height) noexcept
{
    const auto f_point_x = static_cast<float>(point.x);
    const auto f_point_y = static_cast<float>(point.y);

    return {(f_point_x - half_width) / half_width, (half_height - f_point_y) / half_height};
}

Vector2<float> Utility::vectorToGL(const Vector2<float> vector, const float half_width, const float half_height) noexcept
{
    return {static_cast<float>(vector.x) / half_width, static_cast<float>(-vector.y) / half_height};
}