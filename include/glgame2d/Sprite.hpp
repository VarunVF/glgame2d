#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"


namespace glgame2d {


struct Sprite
{
    glm::vec2 position;
    glm::vec2 size;
    Texture texture;

    Sprite(const glm::vec2& position, const glm::vec2& size, const Texture& texture);
    Sprite(const glm::vec2& position, const glm::vec2& size);
    Sprite();

    bool collides(const Sprite& other) const;
};


} // namespace glgame2d
