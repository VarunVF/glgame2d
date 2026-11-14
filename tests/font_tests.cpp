#include "catch2/catch_approx.hpp"
#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Font.hpp>
#include <glgame2d/FontRenderer.hpp>
#include <glgame2d/GLCall.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Window.hpp>


struct CerrCapture
{
    CerrCapture() { old = std::cerr.rdbuf(buffer.rdbuf()); }
    ~CerrCapture() { std::cerr.rdbuf(old); }

    std::string str() const { return buffer.str(); }

    std::ostringstream buffer;
    std::streambuf* old;
};

static std::vector<uint8_t> readFramebuffer(const glgame2d::Window& window)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window.getGLFWWindow(), &width, &height);

    std::vector<uint8_t> pixels;
    pixels.resize(static_cast<size_t>(width) * height * 4);
    GLCall( glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()) );

    return pixels;
}

static size_t countLitPixels(const glgame2d::Window& window)
{
    size_t litPixels = 0;
    std::vector<uint8_t> pixels = readFramebuffer(window);
    for (size_t i = 0; i < pixels.size(); i += 4)
        litPixels += pixels[i] > 128 || pixels[i+1] > 128 || pixels[i+2] > 128;
    return litPixels;
}

static bool leftmostLitX(const glgame2d::Window& window, int& outX)
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


TEST_CASE("Font loads and renders without crashing", "[Font][FontRenderer]")
{
    glgame2d::Window window{ 800, 600, "stb_truetype_example" };
    glgame2d::Renderer renderer{};
    glgame2d::Camera camera{};
    glgame2d::FontRenderer fontRenderer{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    
    SECTION("Invalid font path logs an error")
    {
        CerrCapture capture;
        glgame2d::Font invalidFont{ "this/path/is/invalid", 64.0f };
        REQUIRE(capture.str().find("Failed to load") != std::string::npos);
    }
    
    SECTION("Valid Font renders valid text")
    {
        glgame2d::Font font{ "assets/consola.ttf", 64.0f };
        
        renderer.clear();
        
        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(font, "Hello OpenGL", {-0.5f, 0.0f, 0.0f}, color, 1.0f);
        fontRenderer.renderFrame(font);
        
        window.swapBuffers();
        window.pollEvents();
        
        REQUIRE(font.textureID() != 0);
        REQUIRE(glIsTexture(font.textureID()) == GL_TRUE);
    }

    SECTION("Valid Font handles invalid text")
    {
        glgame2d::Font font{ "assets/consola.ttf", 64.0f };
        
        renderer.clear();
        
        fontRenderer.drawBegin(window, camera);
        // 0x7f is past the last character in the font, so FontRenderer should skip this value.
        fontRenderer.drawText(font, "\x7f", {}, color, 1.0f);
        fontRenderer.renderFrame(font);
        
        window.swapBuffers();
        window.pollEvents();
        
        REQUIRE(font.textureID() != 0);
        REQUIRE(glIsTexture(font.textureID()) == GL_TRUE);
    }

    SECTION("FontRenderer logs an error on missing Fonts")
    {
        CerrCapture capture;
        glgame2d::Font invalidFont{ "this/path/is/invalid", 64.0f };

        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(invalidFont, "A", {}, color, 1.0f);

        REQUIRE(capture.str().find("[FontRenderer] Cannot draw") != std::string::npos);
    }
}

TEST_CASE("Font handles OpenGL state correctly", "[Font][FontRenderer]")
{
    glgame2d::Window window{ 800, 600, "title" };
    glgame2d::Renderer renderer{};
    glgame2d::Camera camera{};
    glgame2d::FontRenderer fontRenderer{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SECTION("Font texture ends up on unit 0 even if another unit was active")
    {
        glgame2d::Font font{ "assets/consola.ttf", 64.0f };

        glActiveTexture(GL_TEXTURE1);   // simulate other code leaving unit 1 active

        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(font, "A", {}, color, 1.0f);
        fontRenderer.renderFrame(font);

        glActiveTexture(GL_TEXTURE0);
        GLint bound = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
        REQUIRE(bound == static_cast<GLint>(font.textureID()));

        glActiveTexture(GL_TEXTURE0);   // leave GL state as we found it
    }

    SECTION("Readback sees drawn text")
    {
        glgame2d::Font font{ "assets/consola.ttf", 64.0f };
    
        renderer.clear();   // black
        REQUIRE(countLitPixels(window) == 0);

        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(font, "AAAA", { -0.5f, 0.0f, 0.0f }, { 1, 1, 1, 1 }, 1.0f);
        fontRenderer.renderFrame(font);
        REQUIRE(countLitPixels(window) > 0);
    }
}

TEST_CASE("Pixel-based image tests", "[Font][FontRenderer]")
{
    glgame2d::Window window{ 800, 600, "title" };
    glgame2d::Renderer renderer{};
    glgame2d::Camera camera{};
    glgame2d::Font font{ "assets/consola.ttf", 64.0f };
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SECTION("Short text after long text draws only the short text")
    {
        auto drawFrame = [&](glgame2d::FontRenderer& fontRenderer, const std::string& text)
        {
            renderer.clear();
            fontRenderer.drawBegin(window, camera);
            fontRenderer.drawText(font, text, { -0.5f, 0.0f, 0.0f }, color, 1.0f);
            fontRenderer.renderFrame(font);
            return countLitPixels(window);
        };

        glgame2d::FontRenderer used{};
        drawFrame(used, "WWWWWWWWWW");       // frame 1
        size_t afterLong = drawFrame(used, "W");   // frame 2
    
        glgame2d::FontRenderer fresh{};
        size_t reference = drawFrame(fresh, "W");
    
        REQUIRE(afterLong == reference);
    }

    SECTION("Font renders correctly when camera moves")
    {
        glgame2d::FontRenderer fontRenderer{};
    
        renderer.clear();
        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(font, "AAAA", { 0.2f, 0.0f, 0.0f }, color, 1.0f);
        fontRenderer.renderFrame(font);
        int x0 = 0;
        REQUIRE(leftmostLitX(window, x0));
    
        renderer.clear();
        camera.move(0.5f, 0.0f);
        fontRenderer.drawBegin(window, camera);
        fontRenderer.drawText(font, "AAAA", { 0.2f, 0.0f, 0.0f }, color, 1.0f);
        fontRenderer.renderFrame(font);
        int x1 = 0;
        REQUIRE(leftmostLitX(window, x1));
    
        int width = 0, height = 0;    
        glfwGetFramebufferSize(window.getGLFWWindow(), &width, &height);
        CAPTURE(x0, x1);
        REQUIRE(x0 - x1 == Catch::Approx(0.5f * height / 2).margin(2));
    }
}
