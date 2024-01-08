#include "pch.hpp"
#include "include/Core/Log.hpp"
#include "include/Core/glCheck.hpp"

void glCheckError(const char* file, const unsigned line, const char* function)
{
    const GLenum error_code = glGetError();

    if (error_code == GL_NO_ERROR)
    {
        return;
    }

    EG_CORE_ERROR("[OpenGL Error]({} ", error_code);
    switch (error_code)
    {
        case GL_INVALID_ENUM:
            EG_CORE_ERROR("Invalid enum");
            break;

        case GL_INVALID_VALUE:
            EG_CORE_ERROR("Invalid value");
            break;

        case GL_INVALID_OPERATION:
            EG_CORE_ERROR("Invalid operation");
            break;

        case GL_STACK_OVERFLOW:
            EG_CORE_ERROR("Stack overflow");
            break;

        case GL_STACK_UNDERFLOW:
            EG_CORE_ERROR("Stack underflow");
            break;

        case GL_OUT_OF_MEMORY:
            EG_CORE_ERROR("Out of memory");
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            EG_CORE_ERROR("Invalid frame buffer operation");
            break;

        case GL_CONTEXT_LOST:
            EG_CORE_ERROR("Context lost");
            break;

        default:
            EG_CORE_ERROR("Unknown error code");
            break;
    }

    EG_CORE_ERROR("): {} {}:{}\n", function, file, line);
}
