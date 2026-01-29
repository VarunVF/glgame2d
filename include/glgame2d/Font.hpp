#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <stb_truetype.h>


namespace glgame2d {


class Font
{
    friend class FontRenderer;

public:
    Font(const std::string& fontFilePath, float fontSize);

private:
    std::vector<uint8_t> setupFont(const std::string& fontFile);
    void setupFontTexture(const std::vector<uint8_t>& fontTextureData);

private:
    // Font Atlas settings:
    static constexpr uint32_t codePointOfFirstChar = 32;        // ASCII of ' ' (Space)
    static constexpr uint32_t charsToIncludeInFontAtlas = 95;   // Include 95 charecters
    static constexpr uint32_t fontAtlasWidth = 512;
    static constexpr uint32_t fontAtlasHeight = 512;

private:
    // Font info
    std::string m_FontFilePath;
    const float m_FontSize;

    // Font Data (This is the data required to render a quad for each glyph)
    stbtt_packedchar m_PackedChars[charsToIncludeInFontAtlas];
    stbtt_aligned_quad m_AlignedQuads[charsToIncludeInFontAtlas];

    uint32_t m_FontTextureID;
};


} // namespace glgame2d
