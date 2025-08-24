#pragma once

#include <iostream>
#include <cassert>

#include <glad/glad.h>


#ifdef NDEBUG
#   define GLCall(x) x
#else
#   define GLCall(x) GLClearError();\
        x;\
        assert(GLLogCall(#x, __FILE__, __LINE__))
#endif // NDEBUG


inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;   // do nothing
}

inline bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "):"
            << function << " " << file << ":" << line;
        return false;
    }
    return true;
}
