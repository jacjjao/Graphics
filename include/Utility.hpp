#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int2.hpp>
#include <functional>

namespace util
{
namespace detail
{
glm::vec2 pointToOpenGL(glm::ivec2 point, float half_width, float half_height);
glm::vec2 vectorToOpenGL(glm::ivec2 vector, float half_width, float half_height);
} // namespace detail

void initialize(unsigned width, unsigned height);

inline std::function<glm::vec2(glm::ivec2)> pointToOpenGL = nullptr;
inline std::function<glm::vec2(glm::ivec2)> vectorToOpenGL = nullptr;

} // namespace util