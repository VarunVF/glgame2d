#pragma once

#include <filesystem>


namespace glgame2d {


class Texture
{
public:
    enum class FilterMode {
        Linear,
        Nearest,
    };

    enum class WrapMode {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
    };

    struct Config {
        FilterMode filterMode = FilterMode::Linear;
        WrapMode wrapMode = WrapMode::Repeat;
    };

public:
    // Create a new texture from a file and upload it to the GPU
    Texture(const std::filesystem::path& assetPath, Config config);
    explicit Texture(const std::filesystem::path& assetPath);
    Texture();
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    static Texture makePlaceholder();

    void bind(unsigned int slot = 0) const;
    unsigned int textureID() const;

private:
    static void initAsPlaceholder(Texture& texture);

private:
    unsigned int m_TextureID;
};


} // namespace glgame2d
