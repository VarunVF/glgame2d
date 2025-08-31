#include "glgame2d/Input.hpp"


namespace glgame2d {


Input::Input()
    : m_Keyboard{}, m_Mouse{}
{
}

void Input::begin(const Window& window)
{
    m_Keyboard.begin(window);
    m_Mouse.begin(window);
}

bool Input::isJustPressed(Key key)
{
    return m_Keyboard.isKeyJustPressed(key);
}

bool Input::isJustReleased(Key key)
{
    return m_Keyboard.isKeyJustReleased(key);
}

bool Input::isHeld(Key key)
{
    return m_Keyboard.isKeyHeld(key);
}

bool Input::isJustPressed(MouseButton key)
{
    return m_Mouse.isButtonJustPressed(key);
}

bool Input::isJustReleased(MouseButton key)
{
    return m_Mouse.isButtonJustReleased(key);
}

bool Input::isHeld(MouseButton key)
{
    return m_Mouse.isButtonHeld(key);
}

Input::MousePosition Input::getMousePosition(const Window& window) const
{
    return m_Mouse.getPosition(window);
}


} // namespace glgame2d
