#include <catch2/catch_test_macros.hpp>

#include <glad/glad.h>

#include "common.hpp"
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Texture.hpp>
#include <glgame2d/Window.hpp>


using Filter = glgame2d::Texture::FilterMode;
using Wrap = glgame2d::Texture::WrapMode;
using Config = glgame2d::Texture::Config;

// Check default filter and wrap modes
// The engine's draw path depends on repeat wrapping,
// so the default must stay on Repeat until that dependency is removed.
static_assert(Config{}.filterMode == Filter::Linear);
static_assert(Config{}.wrapMode == Wrap::Repeat);


namespace {


struct SamplerState {
    GLint minFilter;
    GLint magFilter;
    GLint wrapS;
    GLint wrapT;
};

// Binds the texture to slot 0 and reads its sampler state.
SamplerState readSamplerState(const glgame2d::Texture& texture)
{
    texture.bind();

    SamplerState state{};
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &state.minFilter);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &state.magFilter);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &state.wrapS);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &state.wrapT);
    return state;
}

// Checks that one filter applies to both min and mag, and one wrap mode to both S and T.
void requireSamplerState(const glgame2d::Texture& texture, GLint filter, GLint wrap)
{
    SamplerState state = readSamplerState(texture);
    REQUIRE(state.minFilter == filter);
    REQUIRE(state.magFilter == filter);
    REQUIRE(state.wrapS == wrap);
    REQUIRE(state.wrapT == wrap);
}


} // namespace


TEST_CASE("Texture config", "[Texture]")
{
    glgame2d::Window window = makeWindow();
    glgame2d::Renderer renderer{};

    SECTION("Texture without a config uses linear filtering and repeat")
    {
        glgame2d::Texture texture{ "assets/container.jpg" };
        requireSamplerState(texture, GL_LINEAR, GL_REPEAT);
    }

    SECTION("Linear filtering with clamp to edge")
    {
        glgame2d::Texture texture{ "assets/container.jpg",
            { .filterMode = Filter::Linear, .wrapMode = Wrap::ClampToEdge } };
        requireSamplerState(texture, GL_LINEAR, GL_CLAMP_TO_EDGE);
    }

    SECTION("Nearest filtering with clamp to edge")
    {
        glgame2d::Texture texture{ "assets/container.jpg",
            { .filterMode = Filter::Nearest, .wrapMode = Wrap::ClampToEdge } };
        requireSamplerState(texture, GL_NEAREST, GL_CLAMP_TO_EDGE);
    }

    SECTION("Nearest filtering with repeat")
    {
        glgame2d::Texture texture{ "assets/container.jpg",
            { .filterMode = Filter::Nearest, .wrapMode = Wrap::Repeat } };
        requireSamplerState(texture, GL_NEAREST, GL_REPEAT);
    }

    SECTION("Linear filtering with mirrored repeat")
    {
        glgame2d::Texture texture{ "assets/container.jpg",
            { .filterMode = Filter::Linear, .wrapMode = Wrap::MirroredRepeat } };
        requireSamplerState(texture, GL_LINEAR, GL_MIRRORED_REPEAT);
    }
}
