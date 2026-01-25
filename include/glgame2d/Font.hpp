#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <stb_truetype.h>

#include <glgame2d/Shader.hpp>
#include <glgame2d/Window.hpp>
#include <glgame2d/Renderer.hpp>


struct GLFWwindow;


namespace glgame2d {


class Font
{
private:
    // Struct that represents a vertex drawn
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
    };

public:
    Font(const std::string& fontFilePath, float fontSize);

    void drawBegin();
    void drawText(const Window& window, const std::string& text, glm::vec3 position, glm::vec4 color, float size);
    void renderFrame();

private:
    std::vector<uint8_t> setupFont(const std::string& fontFile);

    void setupVAOAndVBO();
    void setupShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    void setupFontTexture(uint32_t fontAtlasWidth, uint32_t fontAtlasHeight, void* textureData, size_t size);
    void setupViewProjection(float aspectRatio);
    void bindFontTexture(uint8_t slot);
    void useShaderProgram(bool yes);

private:
    // Font Atlas settings:
    static constexpr uint32_t codePointOfFirstChar = 32;        // ASCII of ' ' (Space)
    static constexpr uint32_t charsToIncludeInFontAtlas = 95;   // Include 95 charecters

    static constexpr uint32_t fontAtlasWidth = 512;
    static constexpr uint32_t fontAtlasHeight = 512;

    // VBO size in bytes
    static constexpr size_t vboSize = 600000 * sizeof(Vertex); // Size enough for 600000 vertices (100000 quads)

    static constexpr const char* vertexShaderSrc =
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 a_Position;\n"
        "layout (location = 1) in vec4 a_Color;\n"
        "layout (location = 2) in vec2 a_TexCoord;\n"
        "\n"
        "out vec4 color;\n"
        "out vec2 texCoord;\n"
        "\n"
        "uniform mat4 u_ViewProjectionMat;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = u_ViewProjectionMat * vec4(a_Position, 1.0);\n"
        "    \n"
        "    color = a_Color;\n"
        "    texCoord = a_TexCoord;\n"
        "}\n";

    static constexpr const char* fragmentShaderSrc =
        "#version 330 core\n"
        "\n"
        "in vec4 color;\n"
        "in vec2 texCoord;\n"
        "\n"
        "uniform sampler2D u_FontAtlasTexture;\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    fragColor = vec4(texture(u_FontAtlasTexture, texCoord).r) * color;\n"
        "}\n";

private:
    // Font ---
    
    // Font info
    std::string m_FontFilePath;
    const float m_FontSize;

    // Font Data (This is the data required to render a quad for each glyph)
    stbtt_packedchar m_PackedChars[charsToIncludeInFontAtlas];
    stbtt_aligned_quad m_AlignedQuads[charsToIncludeInFontAtlas];

    // Renderer ---

    // Renderer data
    std::vector<Vertex> m_Vertices;
    uint32_t m_VertexIndex;
    
    // OpenGL Renderer IDs
    uint32_t m_VaoID, m_VboID;
    uint32_t m_ShaderProgramID;
    uint32_t m_FontTextureID;

    // Matrix to upload as uniform
    glm::mat4 m_ViewProjectionMat;
};


} // namespace glgame2d
