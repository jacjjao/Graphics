#include "../include/Utility.hpp"
#include <utility>

glm::vec2 util::detail::pointToOpenGL(const glm::vec2 point, const float half_width, const float half_height)
{
    const auto f_point_x = static_cast<float>(point.x);
    const auto f_point_y = static_cast<float>(point.y);

    return {(f_point_x - half_width) / half_width, (half_height - f_point_y) / half_height};
}

glm::vec2 util::detail::vectorToOpenGL(const glm::vec2 vector, const float half_width, const float half_height)
{
    return {static_cast<float>(vector.x) / half_width, static_cast<float>(vector.y) / half_height};
}

void util::initialize(const unsigned int width, const unsigned int height)
{
    const auto f_width = static_cast<float>(width);
    const auto half_width = static_cast<float>(f_width / 2);
    const auto f_height = static_cast<float>(height);
    const auto half_height = static_cast<float>(f_height / 2);

    pointToOpenGL = [half_width, half_height](auto&& point) {
        return detail::pointToOpenGL(std::forward<decltype(point)>(point), half_width, half_height);
    };
    vectorToOpenGL = [half_width, half_height](auto&& vector) {
        return detail::vectorToOpenGL(std::forward<decltype(vector)>(vector), half_width, half_height);
    };
}