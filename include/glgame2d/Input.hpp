#pragma once

#include "glgame2d/Window.hpp"
#include "glgame2d/Keyboard.hpp"
#include "glgame2d/Mouse.hpp"


namespace glgame2d {


class Input
{
public:
    using Key = Keyboard::Key;
    using MouseButton = Mouse::MouseButton;
    using MousePosition = Mouse::Position;

public:
    Input();

    void begin(const Window& window);

    bool isJustPressed(Key key);
    bool isJustReleased(Key key);
    bool isHeld(Key key);

    bool isJustPressed(MouseButton key);
    bool isJustReleased(MouseButton key);
    bool isHeld(MouseButton key);

    MousePosition getMousePosition(const Window& window) const;

private:
    Keyboard m_Keyboard;
    Mouse m_Mouse;
};


} // namespace glgame2d
