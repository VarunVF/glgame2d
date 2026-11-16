#pragma once

// GLCall: check for OpenGL errors, log and assert on failure.


namespace glgame2d::detail {


void clearGlError();
void checkGlError(const char* expression, const char* file, int line);


} // namespace glgame2d::detail


#ifdef NDEBUG
#   define GLCall(x) (x)
#else
#   define GLCall(x)                                                    \
        do {                                                            \
            glgame2d::detail::clearGlError();                           \
            (x);                                                        \
            glgame2d::detail::checkGlError(#x, __FILE__, __LINE__);     \
        } while (0)
#endif // NDEBUG
