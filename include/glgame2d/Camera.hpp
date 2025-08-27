#pragma once

#include <glm/glm.hpp>

#include "glgame2d/Sprite.hpp"
#include "glgame2d/Window.hpp"


class Camera
{
public:
    Camera();

    /**
     * @brief Move the virtual camera. Upwards and rightwards are taken as positive.
     * 
     * Note, this moves the camera itself and not the view.
     * This transforms the camera's view matrix object.
     * 
     * @param x Horizontal movement amount
     * @param y Vertical movement amount
     */
    void move(float x, float y);

    /**
     * @brief Set the virtual camera scale factor, by the same amount on both the x-axis and y-axis.
     * 
     * @param amount Multiplier value for scale.
     * Values more than 1.0f will magnify, and values less than 1.0f will shrink.
     */
    void setScale(float factor);

    /**
     * @brief Ease the camera towards the given sprite.
     * 
     * @param sprite Sprite to be used as a target for the camera to ease towards.
     */
    void moveEaseTowards(const Sprite& sprite, const Window& window, float easing = 30.0f);

    /**
     * @brief Get the View Matrix object
     * 
     * @return const glm::mat4& 
     */
    const glm::mat4& getViewMatrix() const;

private:
    void rebuildViewMatrix();

private:
    glm::mat4 m_ViewMatrix;
    glm::vec2 m_Position;
    float m_Scale;
};
