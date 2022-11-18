#include "../include/Utility.hpp"

float Utility::window_width = 0.0F;
float Utility::window_height = 0.0F;
float Utility::half_window_width = 0.0F;
float Utility::half_window_height = 0.0F;

void Utility::initialize(const float width, const float height) noexcept
{
    window_width = width;
    window_height = height;
    half_window_width = width / 2.0F;
    half_window_height = height / 2.0F;
}

glm::vec2 Utility::pointToOpenGL(const glm::vec2 point) noexcept
{
    return pointToGL(point, half_window_width, half_window_height);
}

glm::vec2 Utility::vectorToOpenGL(const glm::vec2 vector) noexcept
{
    return vectorToGL(vector, half_window_width, half_window_height);
}

glm::vec2 Utility::pointToGL(const glm::vec2 point, const float half_width, const float half_height) noexcept
{
    const auto f_point_x = static_cast<float>(point.x);
    const auto f_point_y = static_cast<float>(point.y);

    return {(f_point_x - half_width) / half_width, (half_height - f_point_y) / half_height};
}

glm::vec2 Utility::vectorToGL(const glm::vec2 vector, const float half_width, const float half_height) noexcept
{
    return {static_cast<float>(vector.x) / half_width, static_cast<float>(-vector.y) / half_height};
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