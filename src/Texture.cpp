#include "glgame2d/Texture.hpp"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/ensureContextOrAbort.hpp"


namespace glgame2d {


Texture::Texture(const char* assetPath)
    : textureID{ 0 }
{
    ensureContextOrAbort();

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(assetPath, &width, &height, &nrChannels, 0); 
    if (!data)
    {
        std::cerr << "[Texture] Failed to load texture: '" << assetPath << "'\n";
        stbi_image_free(data);
        initAsPlaceholder(*this);
    }
    else
    {
        GLCall( glGenTextures(1, &textureID) );
        GLCall( glActiveTexture(GL_TEXTURE0) );
        GLCall( glBindTexture(GL_TEXTURE_2D, textureID) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
        GLCall( glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data) );
        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );
        stbi_image_free(data);

        std::clog << "[Texture] Loaded '" << assetPath << "' with " << nrChannels << " color channels\n";
    }
}

Texture::Texture(unsigned int textureID)
    : textureID{ textureID }
{
}

Texture::Texture()
    : textureID{ 0 }
{
}

Texture Texture::makePlaceholder()
{
    Texture texture;
    Texture::initAsPlaceholder(texture);
    return texture;
}

void Texture::bind() const
{
    GLCall( glActiveTexture(GL_TEXTURE0) );
    GLCall( glBindTexture(GL_TEXTURE_2D, textureID) );
}

void Texture::initAsPlaceholder(Texture& texture)
{
    // Generate a fallback texture on the fly.
    unsigned char fallbackData[] = {
        255,   0, 255, 255,  64,   0,  64, 255,
         64,   0,  64, 255, 255,   0, 255, 255
    };
    const int fallbackWidth = 2, fallbackHeight = 2;

    GLCall(glGenTextures(1, &texture.textureID));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.textureID));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fallbackWidth, fallbackHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackData));
}


} // namespace glgame2d
