#include "glgame2d/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera()
    : m_ViewMatrix{ 1.0f }
{
}

void Camera::move(float x, float y)
{
    // negate x and y values to simulate camera movement
    m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-x, -y, 0.0f));
}

void Camera::scale(float amount)
{
    m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(amount, amount, 1.0f));
}

const glm::mat4& Camera::getViewMatrix() const
{
    return m_ViewMatrix;
}
