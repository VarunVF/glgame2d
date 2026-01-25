#pragma once


namespace glgame2d {


class Shader
{
public:
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

    unsigned int uniformLocation(const char* uniformName) const;
    void bind() const;

private:
    enum class ShaderType { VERTEX, FRAGMENT };
    static bool validateShaderSource(ShaderType type, unsigned int shaderID);

public:
    unsigned int shaderProgram;
};


} // namespace glgame2d
