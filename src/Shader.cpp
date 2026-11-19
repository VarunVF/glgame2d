#include "glgame2d/Shader.hpp"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/ensureContextOrAbort.hpp"
#include "glgame2d/logging.hpp"



namespace glgame2d {


Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
    : shaderProgram{ 0 }
{
    ensureContextOrAbort();

    unsigned int vertexShader;
    GLCall(vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr));
    GLCall(glCompileShader(vertexShader));
    bool hasVertexShaderCompiled = validateShaderSource(ShaderType::VERTEX, vertexShader);

    unsigned int fragmentShader;
    GLCall(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr));
    GLCall(glCompileShader(fragmentShader));
    bool hasFragmentShaderCompiled = validateShaderSource(ShaderType::FRAGMENT, fragmentShader);

    if (!hasVertexShaderCompiled || !hasFragmentShaderCompiled) {
        return;
    }

    GLCall(shaderProgram = glCreateProgram());
    GLCall(glAttachShader(shaderProgram, vertexShader));
    GLCall(glAttachShader(shaderProgram, fragmentShader));
    GLCall(glLinkProgram(shaderProgram));
    GLCall(glValidateProgram(shaderProgram));

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
}

Shader::Shader()
    : shaderProgram{ 0 }
{
}

Shader Shader::fromFiles(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::string vertexSource = readFile(vertexShaderPath);
    std::string fragmentSource = readFile(fragmentShaderPath);

    if (vertexSource.empty() || fragmentSource.empty())
        return Shader{};
    return Shader(vertexSource.c_str(), fragmentSource.c_str());
}

unsigned int Shader::uniformLocation(const char* uniformName) const
{
    return glGetUniformLocation(shaderProgram, uniformName);
}

void Shader::bind() const
{
    GLCall( glUseProgram(shaderProgram) );
}

bool Shader::validateShaderSource(ShaderType type, unsigned int shaderID)
{
    // Error detection
    int result;
    GLCall( glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result) );
    const char* typeStr = (type == ShaderType::VERTEX ? "vertex" : "fragment");

    if (result == GL_FALSE)
    {
        int length;
        GLCall( glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*)alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(shaderID, length, &length, message) );

        logging::error("[Shader] Failed to compile {} shader!\n{}", typeStr, message);
        GLCall( glDeleteShader(shaderID) );
        return false;
    }
    else
    {
        logging::info("[Shader] Successfully compiled {} shader", typeStr);
        return true;
    }
}

std::string Shader::readFile(const char* filePath)
{
    std::stringstream buffer{};
    std::ifstream file{ filePath };
    if (!file.is_open())
    {
        logging::error("[Shader] Failed to read shader file '{}'", filePath);
    }
    buffer << file.rdbuf();
    return buffer.str();
}


} // namespace glgame2d
