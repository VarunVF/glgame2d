#pragma once

#include <fstream>
#include <string>


namespace glgame2d {


class Shader
{
public:
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    Shader();  // A null shader with an ID of zero.
    static Shader fromFiles(const char* vertexShaderPath, const char* fragmentShaderPath);
    unsigned int uniformLocation(const char* uniformName) const;
    void bind() const;

private:
    enum class ShaderType { VERTEX, FRAGMENT };
    static bool validateShaderSource(ShaderType type, unsigned int shaderID);
    static std::string readFile(const char* filePath);

public:
    unsigned int shaderProgram;
};


} // namespace glgame2d
