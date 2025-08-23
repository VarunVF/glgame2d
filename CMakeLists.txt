cmake_minimum_required(VERSION 3.10)
project(window2d)

# Build GLFW library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("external/glfw/")

# Add our app source
add_executable(app
	"src/main.cpp"
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
)

