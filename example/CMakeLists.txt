cmake_minimum_required(VERSION 3.11)
project(glgame2d_example)

add_executable(app
    "src/main.cpp"
)
target_link_libraries(app PRIVATE glgame2d)
