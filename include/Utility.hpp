#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <functional>

namespace util
{
namespace detail
{
glm::vec2 pointToOpenGL(glm::vec2 point, float half_width, float half_height);
glm::vec2 vectorToOpenGL(glm::vec2 vector, float half_width, float half_height);
} // namespace detail

void initialize(unsigned width, unsigned height);

inline std::function<glm::vec2(glm::vec2)> pointToOpenGL = nullptr;
inline std::function<glm::vec2(glm::vec2)> vectorToOpenGL = nullptr;

} // namespace util