#include "glgame2d/Sprite.hpp"


namespace glgame2d {


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

Rect Sprite::toRect() const
{
    return Rect{
        position.x, position.y,
        size.x, size.y
    };
}

bool Sprite::collides(const Sprite &other) const
{
    return toRect().collides(other.toRect());
}


} // namespace glgame2d
