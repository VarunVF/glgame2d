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

const float& Entity::x() const
{
    return m_Sprite.position.x;
}

const float& Entity::y() const
{
    return m_Sprite.position.y;
}

bool Entity::collides(const Entity& other) const
{
    return m_Sprite.collides(other.m_Sprite);
}

bool Entity::collides(const Sprite& sprite) const
{
    return m_Sprite.collides(sprite);
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
