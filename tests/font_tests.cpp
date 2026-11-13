#include "glgame2d/Camera.hpp"
#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glgame2d/Font.hpp>
#include <glgame2d/FontRenderer.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Window.hpp>


struct CerrCapture {
    CerrCapture() { old = std::cerr.rdbuf(buffer.rdbuf()); }
    ~CerrCapture() { std::cerr.rdbuf(old); }

    std::string str() const { return buffer.str(); }

    std::ostringstream buffer;
    std::streambuf* old;
};

TEST_CASE("Font loads and renders without crashing", "[Font][FontRenderer]")
{
    glgame2d::Window window{ 800, 600, "stb_truetype_example" };
    glgame2d::Renderer renderer{};
    glgame2d::Camera camera{};
    glgame2d::FontRenderer fontRenderer{};
    
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
        
        fontRenderer.drawBegin();
        fontRenderer.drawText(window, camera, font, "This is some text rendered in OpenGL.", {}, {}, 1.0f);
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
        
        fontRenderer.drawBegin();
        fontRenderer.drawText(window, camera, font, "\x7f", {}, {}, 1.0f);
        fontRenderer.renderFrame(font);
        
        window.swapBuffers();
        window.pollEvents();
        
        REQUIRE(font.textureID() != 0);
        REQUIRE(glIsTexture(font.textureID()) == GL_TRUE);
    }

    SECTION("Font texture ends up on unit 0 even if another unit was active")
    {
        glgame2d::Font font{ "assets/consola.ttf", 64.0f };

        glActiveTexture(GL_TEXTURE1);   // simulate other code leaving unit 1 active

        fontRenderer.drawBegin();
        fontRenderer.drawText(window, camera, font, "A", {}, {}, 1.0f);
        fontRenderer.renderFrame(font);

        glActiveTexture(GL_TEXTURE0);
        GLint bound = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
        REQUIRE(bound == static_cast<GLint>(font.textureID()));

        glActiveTexture(GL_TEXTURE0);   // leave GL state as we found it
    }
}
