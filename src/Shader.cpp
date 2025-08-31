#include "glgame2d/Shader.hpp"
#include "glgame2d/GLCall.hpp"

#include <glad/glad.h>


namespace glgame2d {


Shader::Shader()
{
    initShaders(m_VertexShaderSource, m_FragmentShaderSource);
}

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    initShaders(vertexShaderSource, fragmentShaderSource);
}

unsigned int Shader::uniformLocation(const char* uniformName) const
{
    return glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::bind() const
{
    glUseProgram(shaderProgram);
}

void Shader::initShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    unsigned int vertexShader;
    GLCall( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
    GLCall( glShaderSource(vertexShader, 1, &m_VertexShaderSource, nullptr) );
    GLCall( glCompileShader(vertexShader) );
    validateShaderSource(ShaderType::VERTEX, vertexShader);

    unsigned int fragmentShader;
    GLCall( fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) );
    GLCall( glShaderSource(fragmentShader, 1, &m_FragmentShaderSource, nullptr) );
    GLCall( glCompileShader(fragmentShader) );
    validateShaderSource(ShaderType::FRAGMENT, fragmentShader);
    
    GLCall( shaderProgram = glCreateProgram() );
    GLCall( glAttachShader(shaderProgram, vertexShader) );
    GLCall( glAttachShader(shaderProgram, fragmentShader) );
    GLCall( glLinkProgram(shaderProgram) );
    GLCall( glValidateProgram(shaderProgram) );
    
    GLCall( glDeleteShader(vertexShader) );
    GLCall( glDeleteShader(fragmentShader) );

    // Set the texture unit for use in vertex shader
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
}

void Shader::validateShaderSource(ShaderType type, unsigned int shaderID)
{
    // Error detection
    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    const char* typeStr = (type == ShaderType::VERTEX ? "vertex" : "fragment");

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderID, length, &length, message);

        std::cerr << "[Shader] Failed to compile " << typeStr << " shader! " << message << "\n";
        glDeleteShader(shaderID);
    }
    else
        std::clog << "[Shader] Successfully compiled " << typeStr << " shader\n";
}


} // namespace glgame2d
