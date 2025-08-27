#pragma once


class Texture
{
public:
    Texture(const char* assetPath);
    Texture();

    void bind() const;

public:
    unsigned int textureID;
};
