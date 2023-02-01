#include "pch.hpp"
#include "../include/Math.hpp"

namespace Engine
{

    float radians(const float degrees)
    {
        return degrees * static_cast<float>(0.01745329251994329576923690768489); // copy from glm::radians
    }

    Matrix4 ortho(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float zNear,
        const float zFar)
    {
        // stolen from glm::ortho
        auto result = Matrix4::makeIdentity();

        result[0][0] = 2.0F / (right - left);
        result[1][1] = 2.0F / (top - bottom);
        result[2][2] = 2.0F / (zFar - zNear);
        result[0][3] = -(right + left) / (right - left);
        result[1][3] = -(top + bottom) / (top - bottom);
        result[2][3] = -(zFar + zNear) / (zFar - zNear);

        return result;
    }

} // namespace Engine