#pragma once

#include <glm/glm.hpp>

#include "Sprite.hpp"

namespace glgame2d {
    class Tilemap;
} // namespace glgame2d


namespace glgame2d {


class Entity
{
public:
    Entity(const Sprite& sprite, float gravity = -600.0f);
    virtual ~Entity() = default;

    void move(float dx, float dy);
    void update(float deltaTime, const Tilemap& tilemap, const glm::vec2& inputVelocity);
    
    const float& x() const;
    const float& y() const;

    bool collides(const Entity& other) const;
    bool collides(const Sprite& sprite) const;

    const Sprite& getSprite() const;
    Sprite& getSprite();

protected:
    Sprite m_Sprite;
    glm::vec2 m_Displacement;
    glm::vec2 m_Acceleration;
    glm::vec2 m_Velocity;
    float m_AirTime;
};


} // namespace glgame2d
