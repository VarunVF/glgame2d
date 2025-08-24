#include "glgame2d/Sprite.hpp"


Sprite::Sprite(const glm::vec2 &position, const glm::vec2 &size, const Texture& texture)
    : position{ position }, size{ size }, texture{ texture }
{
}

Sprite::Sprite(const glm::vec2 &position, const glm::vec2 &size)
    : position{ position }, size{ size }, texture{}
{
}

Sprite::Sprite()
    : position{}, size{}, texture{}
{
}

bool Sprite::collides(const Sprite &other) const
{
    // Sanity check dimensions
    assert(size.x > 0 && size.y > 0);
    assert(other.size.x > 0 && other.size.y > 0);

    bool overlapX = position.x < other.position.x + other.size.x    // this left bound < other right bound
                 && position.x + size.x > other.position.x;         // this right bound > other left bound
    
    // Note: for y coordinates, downward is positive
    bool overlapY = position.y < other.position.y + other.size.y    // this top bound < other bottom bound (above)
                 && position.y + size.y > other.position.y;         // this bottom bound > other top bound (below)

    return overlapX && overlapY;
}
