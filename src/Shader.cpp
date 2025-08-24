#include "glgame2d/Shader.hpp"
#include "glgame2d/GLCall.hpp"

#include <glad/glad.h>


Shader::Shader()
{
    unsigned int vertexShader;
    GLCall( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
    GLCall( glShaderSource(vertexShader, 1, &m_VertexShaderSource, nullptr) );
    GLCall( glCompileShader(vertexShader) );

    unsigned int fragmentShader;
    GLCall( fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) );
    GLCall( glShaderSource(fragmentShader, 1, &m_FragmentShaderSource, nullptr) );
    GLCall( glCompileShader(fragmentShader) );
    
    GLCall( shaderProgram = glCreateProgram() );
    GLCall( glAttachShader(shaderProgram, vertexShader) );
    GLCall( glAttachShader(shaderProgram, fragmentShader) );
    GLCall( glLinkProgram(shaderProgram) );
    GLCall( glUseProgram(shaderProgram) );
    GLCall( glDeleteShader(vertexShader) );
    GLCall( glDeleteShader(fragmentShader) );
}

unsigned int Shader::uniformLocation(const char* uniformName) const
{
    return glGetUniformLocation(shaderProgram, uniformName);
}
