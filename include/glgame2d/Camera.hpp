#pragma once

#include <glm/glm.hpp>


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
     * @brief Scale the virtual camera view, by the same amount on both the x-axis and y-axis.
     * 
     * @param amount Multiplier value for scale.
     * Values more than 1.0f will magnify, and values less than 1.0f will shrink.
     */
    void scale(float amount);

    /**
     * @brief Get the View Matrix object
     * 
     * @return const glm::mat4& 
     */
    const glm::mat4& getViewMatrix() const;

private:
    glm::mat4 m_ViewMatrix;
};
