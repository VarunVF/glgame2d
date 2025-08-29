#pragma once

#include <bitset>

#include <GLFW/glfw3.h>

#include "glgame2d/Window.hpp"


class Mouse
{
public:
    enum MouseButton {
        BUTTON_1         = GLFW_MOUSE_BUTTON_1,
        BUTTON_2         = GLFW_MOUSE_BUTTON_2,
        BUTTON_3         = GLFW_MOUSE_BUTTON_3,
        BUTTON_4         = GLFW_MOUSE_BUTTON_4,
        BUTTON_5         = GLFW_MOUSE_BUTTON_5,
        BUTTON_6         = GLFW_MOUSE_BUTTON_6,
        BUTTON_7         = GLFW_MOUSE_BUTTON_7,
        BUTTON_8         = GLFW_MOUSE_BUTTON_8,
        BUTTON_LAST      = GLFW_MOUSE_BUTTON_LAST,
        BUTTON_LEFT      = GLFW_MOUSE_BUTTON_LEFT,
        BUTTON_RIGHT     = GLFW_MOUSE_BUTTON_RIGHT,
        BUTTON_MIDDLE    = GLFW_MOUSE_BUTTON_MIDDLE,
    };

    struct Position {
        double xpos, ypos;
    };

public:
    Mouse();

    void begin(const Window& window);

    bool isButtonJustPressed(MouseButton key);
    bool isButtonJustReleased(MouseButton key);
    bool isButtonHeld(MouseButton key);

    Position getPosition(const Window& window) const;

private:
    std::bitset<BUTTON_LAST + 1> m_OldButtons;
    std::bitset<BUTTON_LAST + 1> m_NewButtons;
};
