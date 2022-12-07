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

Vector2f Utility::pointToOpenGL(const Vector2f point) noexcept
{
    return pointToGL(point, half_window_width, half_window_height);
}

Vector2f Utility::vectorToOpenGL(const Vector2f vector) noexcept
{
    return vectorToGL(vector, half_window_width, half_window_height);
}

Vector2f Utility::pointToTexCoord(const Vector2f point, const Vector2f tex_size) noexcept
{
    Vector2f result{};

    result.x = point.x / tex_size.x;
    result.y = (point.y - tex_size.y) / tex_size.y;

    return result;
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

Vector2f Utility::pointToGL(const Vector2f point, const float half_width, const float half_height) noexcept
{
    const auto f_point_x = static_cast<float>(point.x);
    const auto f_point_y = static_cast<float>(point.y);

    return Vector2f{(f_point_x - half_width) / half_width, (half_height - f_point_y) / half_height};
}

Vector2f Utility::vectorToGL(const Vector2f vector, const float half_width, const float half_height) noexcept
{
    return Vector2f{static_cast<float>(vector.x) / half_width, static_cast<float>(-vector.y) / half_height};
}