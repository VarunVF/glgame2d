#include "glgame2d/Keyboard.hpp"

#include <GLFW/glfw3.h>


Keyboard::Keyboard()
    : m_OldKeys{}, m_NewKeys{}
{
}

void Keyboard::begin(const Window& window)
{
    m_OldKeys = m_NewKeys;
    m_NewKeys = 0;

    for (int i = 0; i < KEY_LAST + 1; i++)
        m_NewKeys[i] = glfwGetKey(window.getGLFWWindow(), i) == GLFW_PRESS;
}


bool Keyboard::isKeyJustPressed(Key key)
{
    return !m_OldKeys[key] && m_NewKeys[key];
}

bool Keyboard::isKeyJustReleased(Key key)
{
    return m_OldKeys[key] && !m_NewKeys[key];
}

bool Keyboard::isKeyHeld(Key key)
{
    return m_NewKeys[key];
}
