#include <catch2/catch_test_macros.hpp>

#include <glgame2d/Font.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Window.hpp>


TEST_CASE("Font loads and renders without crashing", "[Font]")
{
    glgame2d::Window window{ 800, 600, "stb_truetype_example" };
    glgame2d::Renderer renderer{};

    SECTION("Invalid Font path does not cause crash")
    {
        glgame2d::Font font{ "this/path/is/invalid", 64.0f };
        
        renderer.clear({ 0.2f, 0.3f, 0.3f, 1.0f });

        font.drawBegin();
        font.drawText(window, "This is some text rendered in OpenGL.", { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
        font.renderFrame();

        window.swapBuffers();
        window.pollEvents();

        REQUIRE(true);
    }
}
