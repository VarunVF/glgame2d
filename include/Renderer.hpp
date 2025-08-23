#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "Sprite.hpp"


class Renderer
{
public:
    Renderer(const Shader& shader)
    {
        // enable blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // get uniform locations
        m_ModelLoc      = shader.uniformLocation("u_Model");
        m_ViewLoc       = shader.uniformLocation("u_View");
        m_ProjectionLoc = shader.uniformLocation("u_Projection");

        // transform matrices
        glm::mat4 view(1.0f);       // camera is identity
        glm::mat4 projection(1.0f); // map to window, identity for now

        // set global uniforms
        glUniformMatrix4fv(m_ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(m_ProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
    
    void clear() const
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
    }

    void drawSprite(const Sprite& sprite, const Shader& shader, const QuadVAO& vao) const
    {
        // set per-sprite model uniform
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(sprite.position, 1.0f));
        glUniformMatrix4fv(m_ModelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // draw call
        glUseProgram(shader.shaderProgram);
		glBindTexture(GL_TEXTURE_2D, sprite.texture.textureID);  // need to set
		glBindVertexArray(vao.VAO);
		glDrawElements(GL_TRIANGLES, vao.indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
    }

private:
    unsigned int m_ModelLoc, m_ViewLoc, m_ProjectionLoc;
};
