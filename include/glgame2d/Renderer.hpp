#pragma once

#include "Camera.hpp"
#include "QuadVAO.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Window.hpp"


namespace glgame2d {


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

    enum PolygonMode {
        POINT,
        LINE,
        FILL
    };
    void setPolygonMode(PolygonMode mode);

    void beginScene(const Camera& camera) const;
    void drawSprite(const Sprite& sprite) const;

private:
    const Shader& m_Shader;
    const Window& m_Window;
    QuadVAO m_QuadVAO;
    unsigned int m_ModelLoc, m_ViewLoc, m_ProjectionLoc;
};


} // namespace glgame2d
