#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>

class Utility
{
public:
    static void initialize(float width, float height) noexcept;
    static glm::vec2 pointToOpenGL(glm::vec2 point) noexcept;
    static glm::vec2 vectorToOpenGL(glm::vec2 vector) noexcept;
    static float getWindowWidth() noexcept;
    static float getWindowHeight() noexcept;
    static float getHalfWindowWidth() noexcept;
    static float getHalfWindowHeight() noexcept;

private:
    static glm::vec2 pointToGL(glm::vec2 point, float half_width, float half_height) noexcept;
    static glm::vec2 vectorToGL(glm::vec2 vector, float half_width, float half_height) noexcept;

    static float window_width, window_height;
    static float half_window_width, half_window_height;
};