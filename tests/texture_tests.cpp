#include <catch2/catch_test_macros.hpp>

#include <glgame2d/QuadVAO.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Texture.hpp>
#include <glgame2d/Window.hpp>


TEST_CASE("Loading Texture with nonexistent path fails gracefully", "[Texture]")
{
	glgame2d::Window window{ 640, 480, "title" };
	glgame2d::QuadVAO vao;
	glgame2d::Shader shader;
	glgame2d::Renderer renderer{ shader, window };

    glgame2d::Texture texture{ "this/path/is/invalid" };
    REQUIRE(texture.textureID != 0);
}

TEST_CASE("Loading Texture with valid path gives valid texture ID", "[Texture]")
{
    glgame2d::Window window{ 640, 480, "title" };
	glgame2d::QuadVAO vao;
	glgame2d::Shader shader;
	glgame2d::Renderer renderer{ shader, window };

    glgame2d::Texture texture{ "assets/container.jpg" };
    REQUIRE(texture.textureID != 0);
}
