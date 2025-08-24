#include <catch2/catch_test_macros.hpp>

#include <glgame2d/QuadVAO.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Texture.hpp>
#include <glgame2d/Window.hpp>


TEST_CASE("Loading Texture with nonexistent path fails gracefully", "[Texture]")
{
	Window window{ 640, 480, "title" };
	QuadVAO vao;
	Shader shader;
	Renderer renderer{ shader, window };

    Texture texture{ "this/path/is/invalid" };
    REQUIRE(texture.textureID != 0);
}

TEST_CASE("Loading Texture with valid path gives valid texture ID", "[Texture]")
{
    Window window{ 640, 480, "title" };
	QuadVAO vao;
	Shader shader;
	Renderer renderer{ shader, window };

    Texture texture{ "assets/container.jpg" };
    REQUIRE(texture.textureID != 0);
}
