#include "glgame2d/Entity.hpp"


namespace glgame2d {


Entity::Entity(const Sprite& sprite)
    : m_Sprite{ sprite }
{
}

void Entity::move(float dx, float dy)
{
    m_Sprite.position.x += dx;
    m_Sprite.position.y += dy;
}

const Sprite& Entity::getSprite() const
{
    return m_Sprite;
}

Sprite& Entity::getSprite()
{
    return m_Sprite;
}


} // namespace glgame2d
