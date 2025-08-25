#pragma once

#include "Camera.hpp"
#include "QuadVAO.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Window.hpp"


class Renderer
{
public:
    struct Color
    {
        float red   = 0.0f;
        float green = 0.0f;
        float blue  = 0.0f;
        float alpha = 1.0f;
    };

public:
    Renderer(const Shader& shader, const Window& window);
    
    void clear() const;
    void clear(const Color& color) const;

    void beginScene(const Camera& camera) const;
    void drawSprite(const Sprite& sprite, const Shader& shader, const QuadVAO& vao) const;

private:
    const Window& m_Window;
    unsigned int m_ModelLoc, m_ViewLoc, m_ProjectionLoc;
};
