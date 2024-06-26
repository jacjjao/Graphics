#pragma once

//#ifdef EG_DEBUG
#ifdef NOT_USING
#define glCheck(expr)                            \
    do                                           \
    {                                            \
        expr;                                    \
        glCheckError(__FILE__, __LINE__, #expr); \
    } while (0)
#else
#define glCheck(expr) expr
#endif

void glCheckError(const char* file, unsigned line, const char* function);
