cmake_minimum_required(VERSION 3.11)
project(window2d)
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# Build GLFW library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/glfw/")

# Add our app source
add_executable(app
	"src/main.cpp"
	"src/QuadVAO.cpp"
	"src/Renderer.cpp"
	"src/Shader.cpp"
	"src/Texture.cpp"
	"src/Window.cpp"

	"src/stb_image.c"
	"src/glad.c"
)
target_include_directories(app PRIVATE
	"include/"
	${OPENGL_INCLUDE_DIR}
)

# Link our app to libraries
find_package(OpenGL REQUIRED)
target_link_libraries(app PRIVATE
	${OPENGL_LIBRARIES}
	glfw
	glm::glm
)

