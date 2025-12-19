#include "glgame2d/Entity.hpp"

#include <array>

#include "glgame2d/Rect.hpp"
#include "glgame2d/Tilemap.hpp"


namespace glgame2d {


Entity::Entity(const Sprite& sprite, float gravity)
    : m_Sprite{ sprite },
    m_Displacement{ sprite.position },
    m_Acceleration{ 0.0f, gravity },
    m_Velocity{ 0.0f, 0.0f },
    m_AirTime{ 0.0f }
{
}

void Entity::move(float dx, float dy)
{
    m_Sprite.position.x += dx;
    m_Sprite.position.y += dy;
}

void Entity::update(float deltaTime, const Tilemap& tilemap, const glm::vec2& inputVelocity)
{
    enum CollisionDirection { TOP, BOTTOM, LEFT, RIGHT };
    std::array<bool, 4> collisions = { false, false, false, false };

    // Euler Integration
    m_Velocity.y = std::fmax(-100.0f, m_Velocity.y + m_Acceleration.y * deltaTime);

    const glm::vec2 frameMovement = (inputVelocity + m_Velocity) * deltaTime;

    // - Update displacement, then set render rect at the end
    // - Move the sprite first
    // - Collide the sprite with each rect
    // - Resolve collision by moving the sprite back

    // Handle horizontal collision
    m_Displacement.x += frameMovement.x;
    Rect entityRect = m_Sprite.toRect();
    entityRect.x = m_Displacement.x;
    auto colliders = tilemap.physicsSpritesAround(entityRect);
    for (int i = 0; i < colliders.count; ++i)
    {
        Rect rect = colliders.items[i].toRect();
        if (entityRect.collides(rect))
        {
            if (frameMovement.x > 0) {
                entityRect.x = rect.x - entityRect.w;
                collisions[RIGHT] = true;
            }
            else if (frameMovement.x < 0) {
                entityRect.x = rect.x + rect.w;
                collisions[LEFT] = true;
            }
            m_Displacement.x = entityRect.x;
        }
    }

    // Handle vertical collision
    m_Displacement.y += frameMovement.y;
    entityRect = m_Sprite.toRect();
    entityRect.y = m_Displacement.y;
    colliders = tilemap.physicsSpritesAround(entityRect);
    for (int i = 0; i < colliders.count; ++i)
    {
        Rect rect = colliders.items[i].toRect();
        if (entityRect.collides(rect))
        {
            if (frameMovement.y > 0) {
                entityRect.y = rect.y - rect.h;
                collisions[TOP] = true;
            }
            else if (frameMovement.y < 0) {
                entityRect.y = rect.y + entityRect.h;
                collisions[BOTTOM] = true;
            }
            m_Displacement.y = entityRect.y;
        }
    }

    // Reset velocities, wall jumps, etc ...
    if (collisions[LEFT] || collisions[RIGHT])
        m_Velocity.x = 0.0f;
    if (collisions[TOP] || collisions[BOTTOM]) {
        m_Velocity.y = 0.0f;
        m_AirTime = 0.0f;
    }
    else {
        m_AirTime += deltaTime;
    }

    // Set position for drawing
    m_Sprite.position = m_Displacement;
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
