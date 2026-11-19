cmake_minimum_required(VERSION 3.11)
project(glgame2d)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GLGAME2D_SANITIZE "Build with ASan and UBSan" OFF)
option(GLGAME2D_BUILD_TESTS "Build tests" ON)
option(GLGAME2D_BUILD_EXAMPLES "Build example executables" ON)

include(FetchContent)

# Obtain GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 		1.0.1
)
FetchContent_MakeAvailable(glm)

# Obtain GLFW (Emscripten ships its own GLFW 3 port on top of the browser,
# so it is only fetched and built for desktop)
if(NOT EMSCRIPTEN)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	FetchContent_Declare(
		glfw
		GIT_REPOSITORY	https://github.com/glfw/glfw.git
		GIT_TAG 		3.4
	)
	FetchContent_MakeAvailable(glfw)
endif()

# Obtain nlohmann_json
FetchContent_Declare(
	nlohmann_json
	GIT_REPOSITORY	https://github.com/nlohmann/json.git
	GIT_TAG			v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

if(GLGAME2D_SANITIZE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(GLGAME2D_SAN_FLAGS
            -fsanitize=address,undefined
            -fno-sanitize-recover=all   # UBSan aborts instead of printing and continuing
            -fno-omit-frame-pointer     # readable stack traces
        )
        add_compile_options(${GLGAME2D_SAN_FLAGS} -g -O1)
        add_link_options(${GLGAME2D_SAN_FLAGS})

		# check operator[] bounds on std::vector and std::array
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            add_compile_options(-D_GLIBCXX_ASSERTIONS)
        endif()
    elseif(MSVC)
        # MSVC only supports ASan
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)
    endif()
endif()


# Add our source
add_library(glgame2d
	"src/Camera.cpp"
	"src/Clock.cpp"
	"src/Entity.cpp"
	"src/Font.cpp"
	"src/FontRenderer.cpp"
	"src/GLCall.cpp"
	"src/Input.cpp"
	"src/Keyboard.cpp"
	"src/logging.cpp"
	"src/Mixer.cpp"
	"src/Mouse.cpp"
	"src/QuadVAO.cpp"
	"src/Renderer.cpp"
	"src/Shader.cpp"
	"src/Sprite.cpp"
	"src/Texture.cpp"
	"src/Tilemap.cpp"
	"src/Window.cpp"

	"src/miniaudio/miniaudio.c"
	"src/stb/stb_image.c"
	"src/stb/stb_truetype.c"
)
target_include_directories(glgame2d PUBLIC
	"include/"
)

if(EMSCRIPTEN)
	# Web: WebGL 2 (OpenGL ES 3.0) and Emscripten's GLFW port. These are
	# link options, PUBLIC so the game's final link picks them up.
	target_link_options(glgame2d PUBLIC
		"SHELL:-sUSE_GLFW=3"
		"SHELL:-sMIN_WEBGL_VERSION=2"
		"SHELL:-sMAX_WEBGL_VERSION=2"
	)
	target_link_libraries(glgame2d PUBLIC
		glm
		nlohmann_json
	)
else()
	# Desktop: OpenGL 3.3 core loaded with glad
	target_sources(glgame2d PRIVATE "src/glad/glad.c")
	find_package(OpenGL REQUIRED)
	target_include_directories(glgame2d PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(glgame2d PUBLIC
		${OPENGL_LIBRARIES}
		glfw
		glm
		nlohmann_json
	)
endif()

# Build tests
if(GLGAME2D_BUILD_TESTS)
	enable_testing()
	add_subdirectory("tests/")
endif()

# Build example projects
if(GLGAME2D_BUILD_EXAMPLES)
	add_subdirectory("examples/")
endif()
