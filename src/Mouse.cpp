#include "glgame2d/Mouse.hpp"


Mouse::Mouse()
    : m_OldButtons{}, m_NewButtons{}
{
}

void Mouse::begin(const Window& window)
{
    m_OldButtons = m_NewButtons;
    m_NewButtons = 0;

    for (int i = 0; i < BUTTON_LAST + 1; i++)
        m_NewButtons[i] = glfwGetMouseButton(window.getGLFWWindow(), i) == GLFW_PRESS;
}

bool Mouse::isButtonJustPressed(MouseButton key)
{
    return !m_OldButtons[key] && m_NewButtons[key];
}

bool Mouse::isButtonJustReleased(MouseButton key)
{
    return m_OldButtons[key] && !m_NewButtons[key];
}

bool Mouse::isButtonHeld(MouseButton key)
{
    return m_NewButtons[key];
}

Mouse::Position Mouse::getPosition(const Window& window) const
{
    Position position{};
    glfwGetCursorPos(window.getGLFWWindow(), &position.xpos, &position.ypos);
    return position;
}
