#include "glm/ext/matrix_float4x4.hpp"
#include <catch2/catch_test_macros.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/logging.hpp>


TEST_CASE("Camera performs move transformations correctly", "[Camera]")
{
    glgame2d::Camera camera;

    SECTION("Default camera has identity matrix")
    {
        REQUIRE(camera.getViewMatrix() == glm::mat4(1.0f));
    }

    SECTION("Moved camera has correct matrix")
    {
        camera.move(23.0f, 19.0f);
        const glm::mat4& mat = camera.getViewMatrix();

        // debug matrix
        for (int i = 0; i < 4; i++)
            glgame2d::logging::info("{}\t{}\t{}\t{}", mat[0][i], mat[1][i], mat[2][i], mat[3][i]);
        glgame2d::logging::info("");

        // note, view is negative of camera
        glm::mat4 expected = {
            glm::vec4{   1.0f,   0.0f, 0.0f, 0.0f },
            glm::vec4{   0.0f,   1.0f, 0.0f, 0.0f },
            glm::vec4{   0.0f,   0.0f, 1.0f, 0.0f },
            glm::vec4{ -23.0f, -19.0f, 0.0f, 1.0f },
        };
        REQUIRE(mat == expected);
    }
}
