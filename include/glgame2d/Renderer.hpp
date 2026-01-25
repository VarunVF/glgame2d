#pragma once

#include <glm/glm.hpp>

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
    Renderer();
    
    void clear() const;
    void clear(const Color& color) const;

    enum PolygonMode {
        POINT,
        LINE,
        FILL
    };
    void setPolygonMode(PolygonMode mode);

    void beginScene(const Camera& camera) const;
    void drawSprite(const Window& window, const Sprite& sprite, const glm::vec4& uvRect) const;
    void drawSprite(const Window& window, const Sprite& sprite) const;

private:
    Shader m_Shader;
    QuadVAO m_QuadVAO;
    unsigned int m_ModelLoc, m_ViewLoc, m_ProjectionLoc;
    unsigned int m_UVrectLoc;
    
private:
    static constexpr const char* m_VertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 a_Pos;\n"
        "\n"
        "uniform mat4 u_Model;\n"
        "uniform mat4 u_View;\n"
        "uniform mat4 u_Projection;\n"
        "uniform vec4 u_UVrect;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 0.0, 1.0);\n"
        "    TexCoord = mix(u_UVrect.xy, u_UVrect.zw, a_Pos);\n"
        "}\n";

    static constexpr const char* m_FragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "\n"
        "uniform sampler2D u_Texture;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(u_Texture, TexCoord);\n"
        "}\n";
};


} // namespace glgame2d
