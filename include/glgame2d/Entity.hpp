#pragma once

#include "Sprite.hpp"


namespace glgame2d {


class Entity
{
public:
    Entity(const Sprite& sprite);
    virtual ~Entity() = default;

    void move(float dx, float dy);
    
    const float& x() const;
    const float& y() const;

    bool collides(const Entity& other) const;
    bool collides(const Sprite& sprite) const;

    const Sprite& getSprite() const;
    Sprite& getSprite();

protected:
    Sprite m_Sprite;
};


} // namespace glgame2d
