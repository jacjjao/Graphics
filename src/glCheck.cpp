#include "../include/pch.hpp"
#include "../include/glCheck.hpp"

#include <glad/glad.h>

void glCheckError(const char* file, const unsigned line, const char* function) noexcept
{
    const GLenum error_code = glGetError();

    if (error_code == GL_NO_ERROR)
    {
        return;
    }

    std::cerr << "[OpenGL Error] (" << error_code << ' ';
    switch (error_code)
    {
        case GL_INVALID_ENUM:
            std::cerr << "Invalid enum";
            break;

        case GL_INVALID_VALUE:
            std::cerr << "Invalid value";
            break;

        case GL_INVALID_OPERATION:
            std::cerr << "Invalid operation";
            break;

        case GL_STACK_OVERFLOW:
            std::cerr << "Stack overflow";
            break;

        case GL_STACK_UNDERFLOW:
            std::cerr << "Stack underflow";
            break;

        case GL_OUT_OF_MEMORY:
            std::cerr << "Out of memory";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "Invalid frame buffer operation";
            break;

        case GL_CONTEXT_LOST:
            std::cerr << "Context lost";
            break;

        default:
            std::cerr << "Unknown error code";
            break;
    }

    std::cerr << "): " << function << ' ' << file << ':' << line << '\n';
}
