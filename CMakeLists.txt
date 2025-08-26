cmake_minimum_required(VERSION 3.11)
project(glgame2d)

include(FetchContent)

# Obtain GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 		1.0.1
)
FetchContent_MakeAvailable(glm)

# Obtain GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY	https://github.com/glfw/glfw.git
	GIT_TAG 		3.4
)
FetchContent_MakeAvailable(glfw)


# Add our source
add_library(glgame2d
	"src/Camera.cpp"
	"src/Clock.cpp"
	"src/QuadVAO.cpp"
	"src/Renderer.cpp"
	"src/Shader.cpp"
	"src/Sprite.cpp"
	"src/Texture.cpp"
	"src/Window.cpp"

	"src/stb_image.c"
	"src/glad.c"
)
target_include_directories(glgame2d PUBLIC
	"include/"
	${OPENGL_INCLUDE_DIR}
)

# Link to libraries
find_package(OpenGL REQUIRED)
target_link_libraries(glgame2d PUBLIC
	${OPENGL_LIBRARIES}
	glfw
	glm
)

# Build tests
add_subdirectory("tests/")

# Build example project
add_subdirectory("example/")
