cmake_minimum_required(VERSION 3.11)
project(glgame2d_examples)

add_executable(game
    "src/game.cpp"
)
target_link_libraries(game PRIVATE glgame2d)

add_executable(font
    "src/font.cpp"
)
target_link_libraries(font PRIVATE glgame2d)
