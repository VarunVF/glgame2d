#pragma once


namespace glgame2d {


class Texture
{
public:
    // Create a new texture from a file and upload it to the GPU
    Texture(const char* assetPath);
    // Take a texture that was already uploaded to the GPU
    Texture(unsigned int textureID);
    Texture();

    void bind() const;

public:
    unsigned int textureID;
};


} // namespace glgame2d
