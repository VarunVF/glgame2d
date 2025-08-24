cmake_minimum_required(VERSION 3.11)
project(glgame2d_tests)

# Obtain Catch2

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY    https://github.com/catchorg/Catch2.git
    GIT_TAG           v3.9.1
)
FetchContent_MakeAvailable(catch2)


# Tests

enable_testing()

add_executable(glgame2d_tests
    "tests.cpp"
    "shader_tests.cpp"
    "sprite_tests.cpp"
    "texture_tests.cpp"
)

target_link_libraries(glgame2d_tests PRIVATE
    glgame2d
    Catch2::Catch2WithMain
)

add_test(NAME glgame2d_tests COMMAND glgame2d_tests)
