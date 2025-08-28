#include "glgame2d/Texture.hpp"

#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "glgame2d/GLCall.hpp"


Texture::Texture(const char* assetPath)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(assetPath, &width, &height, &nrChannels, 0); 
    if (!data)
    {
        // report failure
        std::cerr << "[Texture] Failed to load texture: '" << assetPath << "'\n";
        stbi_image_free(data);

        // load fallback 'error' texture
        assetPath = "assets/fallback_texture.png";
        data = stbi_load(assetPath, &width, &height, &nrChannels, 0);
        if (!data)
            std::cerr << "[Texture] Failed to load fallback texture: '" << assetPath << "', verify assets path.\n";
    }
    
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

Texture::Texture()
    : textureID{ 0 }
{
}

void Texture::bind() const
{
    GLCall( glActiveTexture(GL_TEXTURE0) );
    GLCall( glBindTexture(GL_TEXTURE_2D, textureID) );
}
