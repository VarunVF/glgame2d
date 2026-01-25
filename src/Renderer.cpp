#include "glgame2d/Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/Window.hpp"


namespace glgame2d {


Renderer::Renderer()
    : m_Shader{ m_VertexShaderSource, m_FragmentShaderSource }, m_QuadVAO{}
{
    m_Shader.bind();

    // Set the texture unit for use in fragment shader
    GLCall( glUniform1i(m_Shader.uniformLocation("u_Texture"), 0) );

    // enable blending
    GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    GLCall( glEnable(GL_BLEND) );

    // get uniform locations
    m_ModelLoc      = m_Shader.uniformLocation("u_Model");
    m_ViewLoc       = m_Shader.uniformLocation("u_View");
    m_ProjectionLoc = m_Shader.uniformLocation("u_Projection");
    m_UVrectLoc     = m_Shader.uniformLocation("u_UVrect");
    
    // global projection matrix
    glm::mat4 projection(1.0f);
    projection = glm::translate(projection, glm::vec3(-1.0f, 1.0f, 0.0f));  // move to topleft
    GLCall( glUniformMatrix4fv(m_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection)) );
}

void Renderer::clear() const
{
    GLCall( glClearColor(0.0f, 0.0f, 0.0f, 0.0f) );
    GLCall( glClear(GL_COLOR_BUFFER_BIT) );
}

void Renderer::clear(const Color &color) const
{
    GLCall( glClearColor(color.red, color.green, color.blue, color.alpha) );
    GLCall( glClear(GL_COLOR_BUFFER_BIT) );
}

void Renderer::setPolygonMode(PolygonMode mode)
{
    switch (mode)
    {
        case PolygonMode::FILL:
            GLCall( glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) );
            break;
            
        case PolygonMode::LINE:
            GLCall( glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) );
            break;

        case PolygonMode::POINT:
            GLCall( glPolygonMode(GL_FRONT_AND_BACK, GL_POINT) );
            break;
    }
}

void Renderer::beginScene(const Camera &camera) const
{
    // the correct shader must be bound before uploading uniforms
    m_Shader.bind();
    const auto& view = camera.getViewMatrix();
    GLCall( glUniformMatrix4fv(m_ViewLoc, 1, GL_FALSE, glm::value_ptr(view)) );
}

void Renderer::drawSprite(const Window& window, const Sprite& sprite, const glm::vec4& uvRect) const
{
    // set per-sprite model uniform
    glm::mat4 model(1.0f);
    int width, height;
    window.getSize(&width, &height);
    
    float transX = sprite.position[0] / width * 2;
    float transY = sprite.position[1] / height * 2;
    model = glm::translate(model, glm::vec3(transX, transY, 0.0f));
    
    float scaleX = sprite.size[0] / width * 2;
    float scaleY = sprite.size[1] / height * 2;
    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
    
    // draw call
    m_Shader.bind();
    GLCall( glUniform4f(m_UVrectLoc, uvRect[0], uvRect[1], uvRect[2], uvRect[3]) );
    GLCall( glUniformMatrix4fv(m_ModelLoc, 1, GL_FALSE, glm::value_ptr(model)) );
    sprite.texture.bind();
    GLCall( glBindVertexArray(m_QuadVAO.VAO) );
    GLCall( glDrawElements(GL_TRIANGLES, m_QuadVAO.indicesCount, GL_UNSIGNED_INT, 0) );
    GLCall( glBindVertexArray(0) );
}

void Renderer::drawSprite(const Window& window, const Sprite &sprite) const
{
    drawSprite(window, sprite, glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
}


} // namespace glgame2d
