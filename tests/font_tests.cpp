#include <catch2/catch_test_macros.hpp>

#include <glad/glad.h>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Font.hpp>
#include <glgame2d/GLCall.hpp>
#include <glgame2d/Window.hpp>

#include "common.hpp"


TEST_CASE("Invalid font logs an error", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    CerrCapture capture;
    glgame2d::Font invalidFont{ "this/font/is/invalid", 64.0f };
    REQUIRE(capture.str().find("Failed to load") != std::string::npos);
    REQUIRE(invalidFont.textureID() == 0);
}

TEST_CASE("Valid font path loads", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    glgame2d::Font font{ "assets/consola.ttf", 64.0f };
    REQUIRE(font.textureID() != 0);
}

TEST_CASE("Font texture clamps to edge", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    CerrCapture capture{};
    glgame2d::Font font{ "assets/consola.ttf", 64.0f };

    GLint wrapS, wrapT;
    GLCall( glBindTexture(GL_TEXTURE_2D, font.textureID()) );
    GLCall( glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrapS) );
    GLCall( glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrapT) );
    GLCall( glBindTexture(GL_TEXTURE_2D, 0) );

    REQUIRE(wrapS == GL_CLAMP_TO_EDGE);
    REQUIRE(wrapT == GL_CLAMP_TO_EDGE);
}

TEST_CASE("Empty font file logs an error", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    CerrCapture capture{};
    glgame2d::Font font{ "assets/empty.ttf", 64.0f };
    REQUIRE(capture.str().find("Failed to load") != std::string::npos);
    REQUIRE(font.textureID() == 0);
}

TEST_CASE("Non-font file logs an error", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    CerrCapture capture{};
    glgame2d::Font font{ "assets/container.jpg", 64.0f };
    REQUIRE(capture.str().find("Failed to load") != std::string::npos);
    REQUIRE(font.textureID() == 0);
}

TEST_CASE("Font packing failure logs an error", "[Font]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Camera camera{};
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    CerrCapture capture{};
    glgame2d::Font font{ "assets/consola.ttf", 400.0f };  // load a large size to fail
    REQUIRE(capture.str().find("Glyphs do not fit") != std::string::npos);
    REQUIRE(font.textureID() == 0);
}
