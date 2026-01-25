#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#include <GLFW/glfw3.h>


namespace glgame2d {


inline void ensureContextOrAbort(std::source_location location = std::source_location::current())
{
    if (!glfwGetCurrentContext()) {
        std::cerr << "[OpenGL] "
                  << location.file_name() << '('
                  << location.line() << ':'
                  << location.column() << ") `"
                  << location.function_name() << "`: "
                  << "A valid OpenGL context (window) must exist first\n";
        std::abort();
    }
}


} // namespace glgame2d