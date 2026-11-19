#include "glgame2d/Texture.hpp"

#include <filesystem>
#include <string>

#include "glgame2d/OpenGL.hpp"
#include <stb_image.h>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/ensureContextOrAbort.hpp"
#include "glgame2d/logging.hpp"


namespace glgame2d {


Texture::Texture(const std::filesystem::path& assetPath)
    : m_TextureID{ 0 }
{
    ensureContextOrAbort();

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string assetPathStr = assetPath.string();
    unsigned char *data = stbi_load(assetPathStr.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        logging::warn("[Texture] Failed to load texture: '{}'", assetPathStr);
        stbi_image_free(data);
        initAsPlaceholder(*this);
    }
    else
    {
        GLCall( glGenTextures(1, &m_TextureID) );
        GLCall( glActiveTexture(GL_TEXTURE0) );
        GLCall( glBindTexture(GL_TEXTURE_2D, m_TextureID) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        GLCall( glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data) );
        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );
        stbi_image_free(data);

        logging::info("[Texture] Loaded '{}' with {} color channels", assetPath.string(), nrChannels);
    }
}

Texture::Texture()
    : m_TextureID{ 0 }
{
}

Texture::Texture(Texture&& other) noexcept
{
    m_TextureID = other.m_TextureID;
    other.m_TextureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other) {
        GLCall( glDeleteTextures(1, &m_TextureID) );
        m_TextureID = other.m_TextureID;
        other.m_TextureID = 0;
    }
    return *this;
}

Texture::~Texture() {
    GLCall( glDeleteTextures(1, &m_TextureID) );
}


Texture Texture::makePlaceholder()
{
    Texture texture;
    Texture::initAsPlaceholder(texture);
    return texture;
}

void Texture::bind(unsigned int slot) const
{
    GLCall( glActiveTexture(GL_TEXTURE0 + slot) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_TextureID) );
}

unsigned int Texture::textureID() const {
    return m_TextureID;
}

void Texture::initAsPlaceholder(Texture& texture)
{
    ensureContextOrAbort();

    // Generate a fallback texture on the fly.
    unsigned char fallbackData[] = {
        255,   0, 255, 255,  64,   0,  64, 255,
         64,   0,  64, 255, 255,   0, 255, 255
    };
    const int fallbackWidth = 2, fallbackHeight = 2;

    GLCall(glGenTextures(1, &texture.m_TextureID));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.m_TextureID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fallbackWidth, fallbackHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackData));
}


} // namespace glgame2d
