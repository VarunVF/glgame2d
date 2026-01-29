#include "glgame2d/FontRenderer.hpp"

/*
Thanks to shreyaspranav

This is a simple demo of rendering fonts using stb_truetype.h

The following example demonstrates to render text using batched rendering
i.e., rendering a huge number of quads in one batch rather than once for every quad.

The following demostration doesn't use Element Buffers for the sake of simplicity, the same can be
extended to save a little bit of VRAM
*/

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_truetype.h>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/Shader.hpp"


namespace glgame2d {


FontRenderer::FontRenderer()
    : m_Vertices{},
    m_VertexIndex{ 0 },
    m_VaoID{ 0 }, m_VboID{ 0 },
    m_Shader{ vertexShaderSrc, fragmentShaderSrc },
    m_ViewProjectionMatrixLoc{ m_Shader.uniformLocation("u_ViewProjectionMat") },
    m_FontAtlasTextureLoc{ m_Shader.uniformLocation("u_FontAtlasTexture") },
    m_ViewProjectionMat{ 1.0f }
{
    setupVAOAndVBO();
}

void FontRenderer::drawBegin()
{
    m_VertexIndex = 0;
    useShaderProgram(true);
}

// Adds the required vertices and indices to render:
void FontRenderer::drawText(const Window& window, const Font& font, const std::string& text, glm::vec3 position, glm::vec4 color, float size)
{
    int order[6] = { 0, 1, 2, 0, 2, 3 };
    int currentWindowWidth = 0, currentWindowHeight = 0;
    window.getSize(&currentWindowWidth, &currentWindowHeight);

    float aspectRatio = static_cast<float>(currentWindowWidth) / static_cast<float>(currentWindowHeight);
    setupViewProjection(aspectRatio);
    
    float pixelScale = 2.0f / currentWindowHeight;

    glm::vec3 localPosition = position;

    for (char ch : text)
    {
        // Check if the charecter glyph is in the font atlas.
        if (ch >= font.codePointOfFirstChar && ch <= font.codePointOfFirstChar + font.charsToIncludeInFontAtlas)
        {
            if (m_Vertices.size() <= m_VertexIndex)
                m_Vertices.resize(m_Vertices.size() + 6);

            // Retrive the data that is used to render a glyph of charecter 'ch'
            const stbtt_packedchar* packedChar = &font.m_PackedChars[ch - font.codePointOfFirstChar];
            const stbtt_aligned_quad* alignedQuad = &font.m_AlignedQuads[ch - font.codePointOfFirstChar];

            // The units of the fields of the above structs are in pixels, 
            // convert them to a unit of what we want be multilplying to pixelScale  
            glm::vec2 glyphSize =
            {
                (packedChar->x1 - packedChar->x0) * pixelScale * size,
                (packedChar->y1 - packedChar->y0) * pixelScale * size
            };

            glm::vec2 glyphBoundingBoxBottomLeft =
            {
                localPosition.x + (packedChar->xoff * pixelScale * size),
                localPosition.y - (packedChar->yoff + packedChar->y1 - packedChar->y0) * pixelScale * size
            };

            // The order of vertices of a quad goes top-right, top-left, bottom-left, bottom-right
            glm::vec2 glyphVertices[4] =
            {
                { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y + glyphSize.y },
                { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y + glyphSize.y },
                { glyphBoundingBoxBottomLeft.x, glyphBoundingBoxBottomLeft.y },
                { glyphBoundingBoxBottomLeft.x + glyphSize.x, glyphBoundingBoxBottomLeft.y },
            };

            glm::vec2 glyphTextureCoords[4] =
            {
                { alignedQuad->s1, alignedQuad->t0 },
                { alignedQuad->s0, alignedQuad->t0 },
                { alignedQuad->s0, alignedQuad->t1 },
                { alignedQuad->s1, alignedQuad->t1 },
            };

            // We need to fill the vertex buffer by 6 vertices to render a quad as we are rendering a quad as 2 triangles
            // The order used is in the 'order' array
            // order = [0, 1, 2, 0, 2, 3] is meant to represent 2 triangles: 
            // one by glyphVertices[0], glyphVertices[1], glyphVertices[2] and one by glyphVertices[0], glyphVertices[2], glyphVertices[3]
            for (int i = 0; i < 6; i++)
            {
                m_Vertices[m_VertexIndex + i].position = glm::vec3(glyphVertices[order[i]], position.z);
                m_Vertices[m_VertexIndex + i].color = color;
                m_Vertices[m_VertexIndex + i].texCoord = glyphTextureCoords[order[i]];
            }

            m_VertexIndex += 6;

            // Update the position to render the next glyph specified by packedChar->xadvance.
            localPosition.x += packedChar->xadvance * pixelScale * size;
        }

        // Handle newlines seperately.
        else if (ch == '\n')
        {
            // advance y by m_FontSize, reset x-coordinate
            localPosition.y -= font.m_FontSize * pixelScale * size;
            localPosition.x = position.x;
        }
    }
}

void FontRenderer::setupViewProjection(float aspectRatio)
{
    glm::mat4 projectionMat = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f);
    glm::mat4 viewMat = glm::mat4(1.0f);

    viewMat = glm::translate(viewMat, { 0.0f, 0.0f, 0.0f });
    viewMat = glm::rotate(viewMat, 0.0f, { 1, 0, 0 });
    viewMat = glm::rotate(viewMat, 0.0f, { 0, 1, 0 });
    viewMat = glm::rotate(viewMat, 0.0f, { 0, 0, 1 });
    viewMat = glm::scale(viewMat, { 1.0f, 1.0f, 1.0f });

    m_ViewProjectionMat = projectionMat * viewMat;
}


// OPENGL CODE

void FontRenderer::setupVAOAndVBO()
{
    // The vertex array is set up the following way:
    // Each vertex has consists of 9 floats,
    // -> first 3 determines the position of the vertex
    // -> next 4 floats determines the color of the vertex
    // -> next 2 floats determines the texture coordinates of the vertex.

    // Setting up the VAO and VBO:
    GLCall( glGenBuffers(1, &m_VboID) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_VboID) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_DYNAMIC_DRAW) );

    GLCall( glGenVertexArrays(1, &m_VaoID) );
    GLCall( glBindVertexArray(m_VaoID) );

    // position attribute:
    GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0) );
    GLCall( glEnableVertexAttribArray(0) );

    // color attribute:
    GLCall( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(3 * sizeof(float))) );
    GLCall( glEnableVertexAttribArray(1) );

    // texCoord attribute:
    GLCall( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(7 * sizeof(float))) );
    GLCall( glEnableVertexAttribArray(2) );

    GLCall( glBindVertexArray(0) );
}

void FontRenderer::useShaderProgram(bool use)
{
    GLCall( glUseProgram(use ? m_Shader.shaderProgram : 0) );
}

void FontRenderer::bindFontTexture(const Font& font, uint8_t slot)
{
    GLCall( glBindTexture(GL_TEXTURE_2D, font.m_FontTextureID) );
    GLCall( glActiveTexture(GL_TEXTURE0 + slot) );

    // bind before uploading uniform
    useShaderProgram(true);

    GLCall( glUniform1i(m_FontAtlasTextureLoc, (int)slot) );
}

void FontRenderer::renderFrame(const Font& font)
{
    bindFontTexture(font, 0);

    // The vertex buffer need to be divided into chunks of size 'vboSize',
    // Upload them to the VBO and render
    // This is repeated for every divided chunk of the vertex buffer.

    size_t sizeOfVertices = m_Vertices.size() * sizeof(Vertex);
    uint32_t drawCallCount = static_cast<uint32_t>((sizeOfVertices / vboSize) + 1); // aka number of chunks.
    
    // Render each chunk of vertex data.
    for (uint32_t i = 0; i < drawCallCount; i++)
    {
        const Vertex* data = m_Vertices.data() + i * vboSize;

        uint32_t vertexCount =
            i == drawCallCount - 1 ?
            (sizeOfVertices % vboSize) / sizeof(Vertex) :
            vboSize / (sizeof(Vertex) * 6);

        GLCall( glUniformMatrix4fv(m_ViewProjectionMatrixLoc, 1, GL_TRUE, glm::value_ptr(m_ViewProjectionMat)) );

        GLCall( glBindVertexArray(m_VaoID) );
        GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_VboID) );
        GLCall( glBufferSubData(GL_ARRAY_BUFFER,
            0,
            i == drawCallCount - 1 ? sizeOfVertices % vboSize : vboSize,
            data) );

        GLCall( glDrawArrays(GL_TRIANGLES, 0, vertexCount) );
    }

    // We should clear to prepare for the next draw call.
    m_Vertices.clear();
}


} // namespace glgame2d
