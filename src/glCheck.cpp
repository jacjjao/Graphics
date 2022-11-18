#include "../include/glCheck.hpp"
#include <glad/glad.h>
#include <iostream>

void glCheckError(const char* file, const unsigned line, const char* function)
{
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function << ' ' << file << ':' << line << '\n';
    }
}
