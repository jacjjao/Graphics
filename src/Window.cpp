#include "../include/Window.hpp"

float Window::window_width       = 0.0F;
float Window::window_height      = 0.0F;
float Window::half_window_width  = 0.0F;
float Window::half_window_height = 0.0F;

void Window::initialize(const float width, const float height) noexcept
{
    window_width       = width;
    window_height      = height;
    half_window_width  = width / 2.0F;
    half_window_height = height / 2.0F;
}

Vector3f Window::pointToOpenGL(const Vector3f point) noexcept
{
    return Vector3f{(point.x - half_window_width) / half_window_width,
                    (half_window_height - point.y) / half_window_height,
                    point.z};
}

Vector3f Window::vectorToOpenGL(const Vector3f vector) noexcept
{
    return Vector3f{static_cast<float>(vector.x) / half_window_width,
                    static_cast<float>(-vector.y) / half_window_height,
                    vector.z};
}

float Window::getWindowWidth() noexcept
{
    return window_width;
}

float Window::getWindowHeight() noexcept
{
    return window_height;
}

float Window::getHalfWindowWidth() noexcept
{
    return half_window_width;
}

float Window::getHalfWindowHeight() noexcept
{
    return half_window_height;
}

float Window::radians(const float degrees) noexcept
{
    return degrees * static_cast<float>(0.01745329251994329576923690768489); // copy from glm::radians
}

/* Vector2f Utility::pointToGL(const Vector2f point, const float half_width, const float half_height) noexcept
{
    const auto f_point_x = static_cast<float>(point.x);
    const auto f_point_y = static_cast<float>(point.y);

    return Vector2f{(f_point_x - half_width) / half_width, (half_height - f_point_y) / half_height};
} */

/* Vector2f Utility::vectorToGL(const Vector2f vector, const float half_width, const float half_height) noexcept
{
    return Vector2f{static_cast<float>(vector.x) / half_width, static_cast<float>(-vector.y) / half_height};
} */