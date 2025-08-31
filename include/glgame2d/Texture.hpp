#pragma once


namespace glgame2d {


class Texture
{
public:
    Texture(const char* assetPath);
    Texture();

    void bind() const;

public:
    unsigned int textureID;
};


} // namespace glgame2d
