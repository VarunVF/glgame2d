#include "glgame2d/QuadVAO.hpp"
#include "glgame2d/GLCall.hpp"

#include <glad/glad.h>


QuadVAO::QuadVAO()
{
    indicesCount = sizeof(indices) / sizeof(indices[0]);

    // VAO
    GLCall( glGenVertexArrays(1, &VAO) );
    GLCall( glBindVertexArray(VAO) );

    // VBO
    GLCall( glGenBuffers(1, &VBO) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, VBO) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW) );

    // IBO
    GLCall( glGenBuffers(1, &IBO) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW) );

    GLCall( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0) );
    GLCall( glEnableVertexAttribArray(0) );

    GLCall( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))) );
    GLCall( glEnableVertexAttribArray(1) );

    // Unbind our settings
    GLCall( glBindVertexArray(0) );
}
