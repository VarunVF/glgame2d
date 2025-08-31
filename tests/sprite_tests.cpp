#include <catch2/catch_test_macros.hpp>

#include <glgame2d/Sprite.hpp>


TEST_CASE("Intersecting Sprites must collide", "[Sprite]")
{
    glgame2d::Sprite sprite1 {
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 16.0f, 16.0f }
    };
    glgame2d::Sprite sprite2 {
        glm::vec2{ 5.0f, 0.0f },
        glm::vec2{ 16.0f, 16.0f }
    };

    REQUIRE( sprite1.collides(sprite2) );
}

TEST_CASE("Non-intersecting Sprites must not collide", "[Sprite]")
{
    glgame2d::Sprite sprite1 {
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 16.0f, 16.0f }
    };
    glgame2d::Sprite sprite2 {
        glm::vec2{ 32.0f, 0.0f },
        glm::vec2{ 16.0f, 16.0f }
    };

    REQUIRE_FALSE( sprite1.collides(sprite2) );
}
