#include "glgame2d/Shader.hpp"

#include "glgame2d/OpenGL.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "glgame2d/GLCall.hpp"
#include "glgame2d/ensureContextOrAbort.hpp"
#include "glgame2d/logging.hpp"



namespace glgame2d {


#ifdef __EMSCRIPTEN__
// WebGL 2 only accepts GLSL ES 3.00. The engine's shaders are GLSL 3.30 core,
// which is close enough that swapping the version line and adding a default
// precision is all that is needed.
static std::string toGLSLES(const char* source)
{
    std::string src{ source };
    const std::string desktop = "#version 330 core";
    const std::string web = "#version 300 es\nprecision highp float;\nprecision highp int;";
    if (auto pos = src.find(desktop); pos != std::string::npos)
        src.replace(pos, desktop.size(), web);
    return src;
}
#endif

Shader::Shader(const char* vertexShaderSourceIn, const char* fragmentShaderSourceIn)
    : shaderProgram{ 0 }
{
    ensureContextOrAbort();

#ifdef __EMSCRIPTEN__
    const std::string vertexES = toGLSLES(vertexShaderSourceIn);
    const std::string fragmentES = toGLSLES(fragmentShaderSourceIn);
    const char* vertexShaderSource = vertexES.c_str();
    const char* fragmentShaderSource = fragmentES.c_str();
#else
    const char* vertexShaderSource = vertexShaderSourceIn;
    const char* fragmentShaderSource = fragmentShaderSourceIn;
#endif

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
