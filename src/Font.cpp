#include "glgame2d/Font.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/ensureContextOrAbort.hpp"


namespace glgame2d {


Font::Font(const std::string& fontFilePath, float fontSize)
    : m_FontFilePath{ fontFilePath }, m_FontSize{ fontSize },
    m_PackedChars{}, m_AlignedQuads{},
    m_FontTextureID{ 0 }
{
    ensureContextOrAbort();

    std::vector<uint8_t> fontTextureData = setupFont(m_FontFilePath);
    if (fontTextureData.empty()) {
        std::cerr << "[Font] Failed to load font '" << m_FontFilePath << "'\n";
    }
    else {
        std::cout << "[Font] Loaded font texture from '" << m_FontFilePath << "'\n";
        setupFontTexture(fontTextureData);
    }
}

std::vector<uint8_t> Font::setupFont(const std::string& fontFile)
{
    // Read the font file
    std::ifstream inputStream(fontFile, std::ios::binary);
    if (!inputStream.is_open()) {
        return std::vector<uint8_t>{};
    }

    inputStream.seekg(0, std::ios::end);
    std::streampos fontFileSize = inputStream.tellg();
    inputStream.seekg(0, std::ios::beg);

    std::vector<uint8_t> fontDataBuf;
    fontDataBuf.resize(fontFileSize);
    inputStream.read(reinterpret_cast<char*>(fontDataBuf.data()), fontFileSize);

    stbtt_fontinfo fontInfo = {};

    uint32_t fontCount = stbtt_GetNumberOfFonts(fontDataBuf.data());
    std::cout << "[Font] Font File: " << fontFile << " has " << fontCount << " fonts\n";

    if (!stbtt_InitFont(&fontInfo, fontDataBuf.data(), 0)) {
        std::cerr << "[Font] stbtt_InitFont() Failed!\n";
    }

    std::vector<uint8_t> fontAtlasTextureData;
    fontAtlasTextureData.resize(fontAtlasWidth * fontAtlasHeight);

    stbtt_pack_context ctx;

    stbtt_PackBegin(
        &ctx,                                   // stbtt_pack_context (this call will initialize it) 
        fontAtlasTextureData.data(),            // Font Atlas texture data
        fontAtlasWidth,                         // Width of the font atlas texture
        fontAtlasHeight,                        // Height of the font atlas texture
        0,                                      // Stride in bytes
        1,                                      // Padding between the glyphs
        nullptr);

    stbtt_PackFontRange(
        &ctx,                                   // stbtt_pack_context
        fontDataBuf.data(),                     // Font Atlas texture data
        0,                                      // Font Index                                 
        m_FontSize,                             // Size of font in pixels. (Use STBTT_POINT_SIZE(m_FontSize) to use points) 
        codePointOfFirstChar,                   // Code point of the first charecter
        charsToIncludeInFontAtlas,              // No. of charecters to be included in the font atlas 
        m_PackedChars                           // stbtt_packedchar array, this struct will contain the data to render a glyph
    );
    stbtt_PackEnd(&ctx);

    for (int i = 0; i < charsToIncludeInFontAtlas; i++)
    {
        float unusedX, unusedY;

        stbtt_GetPackedQuad(
            m_PackedChars,                      // Array of stbtt_packedchar
            fontAtlasWidth,                     // Width of the font atlas texture
            fontAtlasHeight,                    // Height of the font atlas texture
            i,                                  // Index of the glyph
            &unusedX, &unusedY,                 // Current position of the glyph in screen pixel coordinates, (not required as we have a different corrdinate system)
            &m_AlignedQuads[i],                 // stbtt_alligned_quad struct. (this struct mainly consists of the texture coordinates)
            0                                   // Align X and Y position to a integer (doesn't matter because we are not using 'unusedX' and 'unusedY')
        );
    }

    return fontAtlasTextureData;
}

void Font::setupFontTexture(const std::vector<uint8_t>& fontTextureData)
{
    // If we got to this point, the data should be valid and non-empty
    assert(fontTextureData.size() == fontAtlasWidth * fontAtlasHeight);

    GLCall(glGenTextures(1, &m_FontTextureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_FontTextureID));

    // The given texture data is a single channel 1 byte per pixel data 
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, fontAtlasWidth, fontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontTextureData.data()));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}



} // namespace glgame2d
