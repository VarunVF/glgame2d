#pragma once

#include <cstdlib>
#include <source_location>

#include <GLFW/glfw3.h>

#include "glgame2d/logging.hpp"


namespace glgame2d {


inline void ensureContextOrAbort(std::source_location location = std::source_location::current())
{
    if (!glfwGetCurrentContext()) {
        logging::error("[OpenGL] {}({}:{}) `{}`: A valid OpenGL context (window) must exist first",
            location.file_name(), location.line(), location.column(), location.function_name());
        std::abort();
    }
}


} // namespace glgame2d
