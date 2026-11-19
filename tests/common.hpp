#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/Window.hpp"
#include "glgame2d/logging.hpp"


// Captures everything logged while it lives, then restores the previous sink.
class LogCapture
{
public:
    LogCapture()
    {
        m_Previous = glgame2d::logging::setSink(
            [this](glgame2d::logging::Level, std::string_view text) {
                m_Text.append(text);
                m_Text.push_back('\n');
            });
    }

    ~LogCapture() { glgame2d::logging::setSink(std::move(m_Previous)); }

    LogCapture(const LogCapture&) = delete;
    LogCapture& operator=(const LogCapture&) = delete;

    std::string str() const { return m_Text; }

private:
    std::string m_Text;
    glgame2d::logging::Sink m_Previous;
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

// Rows are grouped into bands: runs of consecutive rows with at least one lit pixel.
// Returns the top row of each band, ordered from the top of the screen down.
// Rows are framebuffer y (glReadPixels: row 0 is the bottom), so tops are descending.
inline std::vector<int> litBandTops(const glgame2d::Window& window)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.getGLFWWindow(), &width, &height);
    std::vector<uint8_t> pixels = readFramebuffer(window);

    auto rowLit = [&](int y) {
        for (int x = 0; x < width; x++) {
            size_t i = (static_cast<size_t>(y) * width + x) * 4;
            if (pixels[i] > 128 || pixels[i+1] > 128 || pixels[i+2] > 128)
                return true;
        }
        return false;
    };

    std::vector<int> tops;
    bool inBand = false;
    for (int y = height - 1; y >= 0; y--) {
        bool lit = rowLit(y);
        if (lit && !inBand) tops.push_back(y);
        inBand = lit;
    }
    return tops;
}
