#pragma once

#include <cassert>
#include <iostream>
#include <unordered_map>

#include <glad/glad.h>


#ifdef NDEBUG
#   define GLCall(x) x
#else
#   define GLCall(x) GLClearError();\
        x;\
        assert(GLLogCall(#x, __FILE__, __LINE__))
#endif // NDEBUG


static std::unordered_map<GLenum, const char*> errorMessages {
    { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
    { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
    { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
    { GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW" },
    { GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW" },
    { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
    { 0x0507, "GL_CONTEXT_LOST" },
    { 0x8031, "GL_TABLE_TOO_LARGE" },
};


inline void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;   // do nothing
}

inline bool GLLogCall(const char* function, const char* file, int line)
{
    bool wasSuccessful = true;
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << " - " << errorMessages[error] << "): "
                  << function << " " << file << ":" << line << "\n";
        wasSuccessful = false;
    }
    return wasSuccessful;
}
