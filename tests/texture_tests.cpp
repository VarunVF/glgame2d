#include <catch2/catch_test_macros.hpp>

#include <type_traits>

#include <glad/glad.h>

#include "common.hpp"
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Texture.hpp>
#include <glgame2d/Window.hpp>


TEST_CASE("Texture loading", "[Texture]")
{
	glgame2d::Window window = makeWindow();
	glgame2d::Renderer renderer{};

    SECTION("Placeholder texture works")
    {
        glgame2d::Texture texture = glgame2d::Texture::makePlaceholder();
        REQUIRE(texture.textureID() != 0);        
    }

    SECTION("Loading Texture with nonexistent path makes placeholder")
    {
        glgame2d::Texture texture{ "this/path/is/invalid" };
        REQUIRE(texture.textureID() != 0);
    }

    SECTION("Loading Texture with valid path gives valid texture ID")
    {
        glgame2d::Texture texture{ "assets/container.jpg" };
        REQUIRE(texture.textureID() != 0);

        GLint width = 0;
        texture.bind();
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        REQUIRE(width == 512);
    }
}

TEST_CASE("Texture semantics", "[Texture]")
{
    glgame2d::Window window = makeWindow();
	glgame2d::Renderer renderer{};

    SECTION("Texture should be move-only")
    {
        static_assert(std::is_nothrow_move_constructible_v<glgame2d::Texture>);
        static_assert(std::is_nothrow_move_assignable_v<glgame2d::Texture>);
        static_assert(!std::is_copy_constructible_v<glgame2d::Texture>);
        static_assert(!std::is_copy_assignable_v<glgame2d::Texture>);
    }

    SECTION("Move constructor")
    {
        glgame2d::Texture a{ "assets/container.jpg" };
        unsigned int movingID = a.textureID();
        REQUIRE(movingID != 0);

        glgame2d::Texture b = std::move(a);
        REQUIRE(b.textureID() == movingID);
        REQUIRE(glIsTexture(movingID) == GL_TRUE);
        REQUIRE(a.textureID() == 0);
    }

    SECTION("Move assignment operator")
    {
        glgame2d::Texture a{ "assets/container.jpg" };
        unsigned int movingID = a.textureID();
        REQUIRE(movingID != 0);

        glgame2d::Texture b{ "assets/awesomeface.png" };
        unsigned int expiringID = b.textureID();
        REQUIRE(expiringID != 0);

        b = std::move(a);
        REQUIRE(glIsTexture(expiringID) == GL_FALSE);
        REQUIRE(b.textureID() == movingID);
        REQUIRE(glIsTexture(movingID) == GL_TRUE);
        REQUIRE(a.textureID() == 0);
    }

    SECTION("Texture resource should be freed it goes when out of scope")
    {
        unsigned int id = 0;
        {
            glgame2d::Texture texture{ "assets/container.jpg" };
            id = texture.textureID();
        }
    
        REQUIRE(glIsTexture(id) == GL_FALSE);
    }
}
