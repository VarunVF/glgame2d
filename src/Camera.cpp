#include "glgame2d/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera()
    : m_ViewMatrix{ 1.0f }, m_Position{ 0.0f, 0.0f }, m_Scale{ 1.0f }
{
}

void Camera::move(float x, float y)
{
    // negate x and y values to simulate camera movement
    m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-x, -y, 0.0f));
}

void Camera::setScale(float factor)
{
    m_Scale = factor;
}

void Camera::moveEaseTowards(const Sprite& sprite, const Window& window, float easing)
{
    int width, height;
    window.getSize(&width, &height);

    // Avoid NaN (size is zero if window is minimized)
    if (width == 0 || height == 0)
        return;

    // Target the sprite to appear at the center of the screen
    glm::vec2 targetCenter {
        sprite.position.x - width / 2.0f,
        sprite.position.y + height / 2.0f,
    };

    // Convert to NDC range
    targetCenter.x /= width  / 2.0f;
    targetCenter.y /= height / 2.0f;

    m_Position += (targetCenter - m_Position) / easing;
    rebuildViewMatrix();
}

void Camera::rebuildViewMatrix()
{
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-m_Position, 0.0f));
    m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(m_Scale, m_Scale, 0.0f));
}

const glm::mat4& Camera::getViewMatrix() const
{
    return m_ViewMatrix;
}
