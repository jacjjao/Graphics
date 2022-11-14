#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <fmt/core.h>

void glCheckError(const char* file, const unsigned line, const char* function)
{
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        fmt::print(stderr, "[OpenGL Error] ({}): {} {}:{}\n", error, function, file, line);
    }
}
