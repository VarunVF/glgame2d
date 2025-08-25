#include "glgame2d/Renderer.hpp"
#include "glgame2d/GLCall.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Renderer::Renderer(const Shader& shader, const Window& window)
    : m_Window{ window }
{
    // enable blending
    GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    GLCall( glEnable(GL_BLEND) );

    // get uniform locations
    m_ModelLoc      = shader.uniformLocation("u_Model");
    m_ViewLoc       = shader.uniformLocation("u_View");
    m_ProjectionLoc = shader.uniformLocation("u_Projection");

    // global projection matrix
    glm::mat4 projection(1.0f);
    projection = glm::translate(projection, glm::vec3(-1.0f, 1.0f, 1.0f));
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

void Renderer::beginScene(const Camera &camera) const
{
    const auto& view = camera.getViewMatrix();
    GLCall( glUniformMatrix4fv(m_ViewLoc, 1, GL_FALSE, glm::value_ptr(view)) );
}

void Renderer::drawSprite(const Sprite& sprite, const Shader& shader, const QuadVAO& vao) const
{
    // set per-sprite model uniform
    glm::mat4 model(1.0f);
    int width, height;
    m_Window.getSize(&width, &height);
    float scaleX = sprite.size[0] / width * 2;
    float scaleY = sprite.size[1] / height * 2;
    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
    model = glm::translate(model, glm::vec3(sprite.position, 0.0f));

    // debug matrix
    // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < 4; j++)
    //         std::cout << model[j][i] << "\t";
    //     std::cout << "\n";
    // }
    // std::cout << "\n";

    GLCall( glUniformMatrix4fv(m_ModelLoc, 1, GL_FALSE, glm::value_ptr(model)) );

    // draw call
    GLCall( glUseProgram(shader.shaderProgram) );
    GLCall( glBindTexture(GL_TEXTURE_2D, sprite.texture.textureID) );
    GLCall( glBindVertexArray(vao.VAO) );
    GLCall( glDrawElements(GL_TRIANGLES, vao.indicesCount, GL_UNSIGNED_INT, 0) );
    GLCall( glBindVertexArray(0) );
}
