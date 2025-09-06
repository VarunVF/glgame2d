#include "glgame2d/Tilemap.hpp"

#include <cassert>

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>


namespace glgame2d {


Tilemap::Tilemap(const char* tilemapPath)
    : m_MapInfo{}, m_Layers{}
{
    // Assume the following when loading:
    // - dim. of map as a whole == dim. of each individual layer
    // - index in j["layers"] array == Z index of that layer
    // - tile dim. as reported in map == tile dim. as reported in each tileset

    namespace fs = std::filesystem;

    std::ifstream stream{ tilemapPath };
    nlohmann::json j = nlohmann::json::parse(stream);

    // load map info
    m_MapInfo.width = j["width"];
    m_MapInfo.height = j["height"];
    m_MapInfo.tileWidth = j["tilewidth"];
    m_MapInfo.tileHeight = j["tileheight"];

    // load tilesets
    for (const auto& tileset : j["tilesets"])
    {
        TileGID firstgid = tileset["firstgid"];
        std::string source = tileset["source"];
        
        fs::path mapPath{ tilemapPath };
        fs::path parentPath{ mapPath.parent_path() };
        fs::path tilesetInfoPath{ parentPath / source };
        std::ifstream tilesetInfoStream{ tilesetInfoPath };
        nlohmann::json tilesetInfoJson = nlohmann::json::parse(tilesetInfoStream);

        std::string imagePathStr = tilesetInfoJson["image"];
        fs::path imagePath = fs::canonical(parentPath / imagePathStr);
        m_Tilesets.emplace_back(
            firstgid,
            imagePath,
            tilesetInfoJson["columns"],
            tilesetInfoJson["imagewidth"],
            tilesetInfoJson["imageheight"],
            tilesetInfoJson["tilecount"]
        );
    }

    // load map, respecting Z order
    int zOrder = 0;
    m_Layers.resize(j["layers"].size());
    for (const auto& layerJson : j["layers"])
    {
        auto& currentLayer = m_Layers[zOrder];
        currentLayer.reserve(layerJson["data"].size());

        for (TileGID gid : layerJson["data"])
            currentLayer.emplace_back(gid);
        
        ++zOrder;
    }

    // sort tilesets by firstgid
    auto compareTilesets = [](const Tileset& lhs, const Tileset& rhs)
                           { return lhs.firstgid < rhs.firstgid; };
    std::sort(m_Tilesets.begin(), m_Tilesets.end(), compareTilesets);
}

const Tilemap::Tileset& Tilemap::getTilesetByGID(TileGID gid) const
{
    assert(!m_Tilesets.empty());

    if (gid <= 0)
    {
        std::cerr << "[Tilemap] Invalid Tile GID " << gid << "\n";
        return m_Tilesets.front();
    }

    // linear search, for now
    for (int i = 0; i < m_Tilesets.size() - 1; i++)
    {
        if (m_Tilesets[i].firstgid <= gid && gid < m_Tilesets[i+1].firstgid)
            return m_Tilesets[i];
    }

    return m_Tilesets.back();
}

void glgame2d::Tilemap::renderTileFromTileset(
    const Renderer& renderer, const Tileset& tileset,
    const glm::vec4& uvRect, const glm::vec2& position) const
{
    float sizePx = static_cast<float>(tileset.imagewidth) / static_cast<float>(tileset.columns);
    glm::vec2 size{ sizePx, sizePx };
    Sprite sprite{ position, size, tileset.texture };
    renderer.drawSprite(sprite, uvRect);
}

void Tilemap::render(const Renderer& renderer) const
{
    for (const auto& layer : m_Layers)
    {
        for (int dataIdx = 0; dataIdx < layer.size(); dataIdx++)
        {
            TileGID gid = layer[dataIdx];
            if (gid == 0) continue; // no tile present

            const auto& tileset = getTilesetByGID(gid);
            int tilesetUVIndex = gid - tileset.firstgid;
            assert(tilesetUVIndex >= 0);

            glm::vec2 uvIndices{
                tilesetUVIndex % tileset.rows,
                tilesetUVIndex / tileset.columns
            };
            float unit = 1.0f / tileset.columns;
            glm::vec4 uvRect{
                 unit * uvIndices.x,
                -unit * uvIndices.y,
                unit * (1 + uvIndices.x),
                unit * (1 - uvIndices.y)
            };

            glm::vec2 position{
                dataIdx % m_MapInfo.width *  m_MapInfo.tileWidth,
                dataIdx / m_MapInfo.width * -m_MapInfo.tileHeight
            };

            renderTileFromTileset(renderer, tileset, uvRect, position);
        }
    }
}

} // namespace glgame2d
