#include "../include/Math.hpp"

float radians(const float degrees) noexcept
{
    return degrees * static_cast<float>(0.01745329251994329576923690768489); // copy from glm::radians
}