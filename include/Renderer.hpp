#pragma once

#include "QuadVAO.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Window.hpp"


class Renderer
{
public:
    Renderer(const Shader& shader, const Window& window);
    
    void clear() const;
    void drawSprite(const Sprite& sprite, const Shader& shader, const QuadVAO& vao) const;

private:
    const Window& m_Window;
    unsigned int m_ModelLoc, m_ViewLoc, m_ProjectionLoc;
};
