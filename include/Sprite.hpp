#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"


struct Sprite
{
    glm::vec2 position;
    glm::vec2 size;
    Texture texture;
};
