#include "glgame2d/GLCall.hpp"

#include <cassert>
#include <unordered_map>

#include <glad/glad.h>

#include "glgame2d/logging.hpp"


namespace glgame2d::detail {


void clearGlError()
{
    while (glGetError() != GL_NO_ERROR)
        ;   // do nothing
}

void checkGlError(const char* expression, const char* file, int line)
{
    static const std::unordered_map<GLenum, const char*> errorMessages {
        { GL_INVALID_ENUM, "GL_INVALID_ENUM" },
        { GL_INVALID_VALUE, "GL_INVALID_VALUE" },
        { GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
        { GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW" },
        { GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW" },
        { GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" },
        { 0x0507, "GL_CONTEXT_LOST" },
        { 0x8031, "GL_TABLE_TOO_LARGE" },
    };

    bool wasSuccessful = true;
    while (GLenum error = glGetError())
    {
        glgame2d::logging::error("[OpenGL] ({} - {}): {} {}:{}",
            error, errorMessages.at(error), expression, file, line);
        wasSuccessful = false;
    }
    
    assert(wasSuccessful);
}


} // namespace glgame2d::detail
