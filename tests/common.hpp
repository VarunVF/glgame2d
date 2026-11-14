#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "glgame2d/GLCall.hpp"
#include "GLFW/glfw3.h"

#include "glgame2d/Window.hpp"


struct CerrCapture
{
    CerrCapture() { old = std::cerr.rdbuf(buffer.rdbuf()); }
    ~CerrCapture() { std::cerr.rdbuf(old); }

    std::string str() const { return buffer.str(); }

    std::ostringstream buffer;
    std::streambuf* old;
};


inline glgame2d::Window makeWindow(int width = 640, int height = 480)
{
    glgame2d::Window window{ width, height, "testing", glgame2d::Window::WINDOWED, true };
    return window;
}

inline std::vector<uint8_t> readFramebuffer(const glgame2d::Window& window)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.getGLFWWindow(), &width, &height);

    std::vector<uint8_t> pixels;
    pixels.resize(static_cast<size_t>(width) * height * 4);
    GLCall( glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()) );

    return pixels;
}

inline size_t countLitPixels(const glgame2d::Window& window)
{
    size_t litPixels = 0;
    std::vector<uint8_t> pixels = readFramebuffer(window);
    for (size_t i = 0; i < pixels.size(); i += 4)
        litPixels += pixels[i] > 128 || pixels[i+1] > 128 || pixels[i+2] > 128;
    return litPixels;
}

inline bool leftmostLitX(const glgame2d::Window& window, int& outX)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.getGLFWWindow(), &width, &height);
    std::vector<uint8_t> pixels = readFramebuffer(window);

    int minX = width;
    for (size_t i = 0; i < pixels.size(); i += 4) {
        if (pixels[i] > 128 || pixels[i+1] > 128 || pixels[i+2] > 128) {
            int x = static_cast<int>((i / 4) % width);
            minX = std::min(minX, x);
        }
    }
    if (minX == width) return false;   // nothing lit
    outX = minX;
    return true;
}
