cmake_minimum_required(VERSION 3.11)
project(glgame2d_examples)

set(GAME_SOURCES "src/game.cpp")
set(FONT_SOURCES "src/font.cpp")

# Only include the icon resource if we are on Windows
if(WIN32)
    list(APPEND GAME_SOURCES "resources.rc")
    list(APPEND FONT_SOURCES "resources.rc")
endif()

add_executable(game ${GAME_SOURCES})
target_link_libraries(game PRIVATE glgame2d)

add_executable(font ${FONT_SOURCES})
target_link_libraries(font PRIVATE glgame2d)
