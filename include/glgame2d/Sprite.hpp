#pragma once

#include <glm/glm.hpp>

#include "Rect.hpp"
#include "Texture.hpp"


namespace glgame2d {


struct Sprite
{
    Sprite(const glm::vec2& position, const glm::vec2& size, const Texture& texture);
    Sprite(const glm::vec2& position, const glm::vec2& size);
    Sprite();

    Rect toRect() const;
    bool collides(const Sprite& other) const;
    
    glm::vec2 position;
    glm::vec2 size;
    Texture texture;
};


} // namespace glgame2d
