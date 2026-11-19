#pragma once

// Single place that decides where OpenGL comes from.
//
// Desktop: OpenGL 3.3 core, functions loaded at runtime by glad.
// Web (Emscripten): WebGL 2, which is OpenGL ES 3.0. The functions are linked
// directly and there is nothing to load.
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/glad.h>
#endif
