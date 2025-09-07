#include "glgame2d/Tilemap.hpp"

#include <cassert>

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>


namespace glgame2d {


Tilemap::Tilemap(const char* tilemapPath)
    : m_MapInfo{}, m_TileLayers{}
{
    // Assume the following when loading:
    // - index in j["layers"] array == Z index of that layer

    namespace fs = std::filesystem;

    std::ifstream stream{ tilemapPath };
    nlohmann::json j = nlohmann::json::parse(stream);

    // load map info
    m_MapInfo.width = j["width"];
    m_MapInfo.height = j["height"];
    m_MapInfo.tileWidth = j["tilewidth"];
    m_MapInfo.tileHeight = j["tileheight"];
    m_MapInfo.layerCount = j["layers"].size();

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
        float tilesetImageHeight = tilesetInfoJson["imageheight"];
        float tilesetTileHeight = tilesetInfoJson["tileheight"];
        m_Tilesets.emplace_back(
            firstgid,
            imagePath,
            tilesetInfoJson["columns"],
            tilesetImageHeight / tilesetTileHeight,
            tilesetInfoJson["imagewidth"],
            tilesetInfoJson["imageheight"],
            tilesetInfoJson["tilecount"]
        );
    }

    // load map, respecting Z order
    m_TileLayers.resize(m_MapInfo.layerCount);
    m_ObjectLayers.resize(m_MapInfo.layerCount);
    for (int zOrder = 0; zOrder < m_MapInfo.layerCount; zOrder++)
    {
        const auto& layerJson = j["layers"][zOrder];

        if (layerJson["type"] == "tilelayer")
        {
            if (layerJson["width"] != m_MapInfo.width || layerJson["height"] != m_MapInfo.height)
                std::cerr << "[Tilemap] Warning: Layer dimensions do not match map dimensions\n";
            
            auto& currentLayer = m_TileLayers[zOrder];
            currentLayer.reserve(layerJson["data"].size());
            
            for (TileGID gid : layerJson["data"])
                currentLayer.emplace_back(gid);
        }
        else if (layerJson["type"] == "objectgroup")
        {
            for (const auto& object : layerJson["objects"])
                m_ObjectLayers[zOrder].emplace_back(
                    object["gid"],
                    object["width"], object["height"],
                    object["x"], object["y"]
                );
        }
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

glm::vec4 Tilemap::computeTileUV(const Tileset& tileset, TileGID gid)
{
    int tilesetIndex = gid - tileset.firstgid;
    assert(tilesetIndex >= 0);

    float xIndex = tilesetIndex % tileset.columns;
    float yIndex = tilesetIndex / tileset.columns;

    float uPerTile = 1.0f / tileset.columns;
    float vPerTile = 1.0f / tileset.rows;
    
    return glm::vec4{
         uPerTile * xIndex,
        -vPerTile * yIndex,
        uPerTile * (1 + xIndex),
        vPerTile * (1 - yIndex)
    };
}

void glgame2d::Tilemap::renderTileFromTileset(
    const Renderer& renderer, const Tileset& tileset,
    const glm::vec4& uvRect, const glm::vec2& position) const
{
    glm::vec2 size{
        static_cast<float>(tileset.imagewidth)  / static_cast<float>(tileset.columns),
        static_cast<float>(tileset.imageheight) / static_cast<float>(tileset.rows)
    };

    Sprite sprite{ position, size, tileset.texture };
    
    renderer.drawSprite(sprite, uvRect);
}

void Tilemap::render(const Renderer& renderer) const
{
    for (int i = 0; i < m_MapInfo.layerCount; i++)
    {
        const auto& objectLayer = m_ObjectLayers[i];
        for (const auto& object : objectLayer)
        {
            const Tileset& tileset = getTilesetByGID(object.tilegid);
            glm::vec4 uvRect{ computeTileUV(tileset, object.tilegid) };
            glm::vec2 position{
                object.x,
                -(object.y - object.height)
            };
            renderTileFromTileset(renderer, tileset, uvRect, position);
        }

        const auto& tileLayer = m_TileLayers[i];
        for (int dataIdx = 0; dataIdx < tileLayer.size(); dataIdx++)
        {
            TileGID gid = tileLayer[dataIdx];
            if (gid == 0) continue; // no tile present

            const auto& tileset = getTilesetByGID(gid);
            glm::vec4 uvRect{ computeTileUV(tileset, gid) };
            glm::vec2 position{
                dataIdx % m_MapInfo.width *  m_MapInfo.tileWidth,
                dataIdx / m_MapInfo.width * -m_MapInfo.tileHeight
            };

            renderTileFromTileset(renderer, tileset, uvRect, position);
        }
    }
}

} // namespace glgame2d
