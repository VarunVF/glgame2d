#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include <glgame2d/Camera.hpp>


TEST_CASE("Camera performs move transformations correctly", "[Camera]")
{
    Camera camera;
    const auto& mat = camera.getViewMatrix();

    SECTION("Default camera has identity matrix")
    {
        // check diagonal, all other elements should be zero
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                if (row == col)
                    REQUIRE(mat[col][row] == 1.0f);
                else
                    REQUIRE(mat[col][row] == 0.0f);
            }
        }
    }

    SECTION("Moved camera has correct matrix")
    {
        camera.move(23.0f, 19.0f);

        // debug matrix
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                std::cerr << mat[j][i] << "\t";
            std::cerr << "\n";
        }
        std::cerr << "\n";

        // check diagonal
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                if (row == col)
                    REQUIRE(mat[col][row] == 1.0f);
            }
        }

        // check last column, view is negative of camera
        REQUIRE( mat[3][0] == -23.0f );
        REQUIRE( mat[3][1] == -19.0f );
        REQUIRE( mat[3][2] ==   0.0f );
    }
}
