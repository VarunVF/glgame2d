#pragma once


class Shader
{
public:
    Shader();
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

    unsigned int uniformLocation(const char* uniformName) const;
    void bind() const;

private:
    void initShaders(const char* vertexShaderSource, const char* fragmentShaderSource);
    
    enum class ShaderType { VERTEX, FRAGMENT };
    static void validateShaderSource(ShaderType type, unsigned int shaderID);

public:
    unsigned int shaderProgram;

private:
    const char* m_VertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "\n"
        "uniform mat4 u_Model;\n"
        "uniform mat4 u_View;\n"
        "uniform mat4 u_Projection;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";

    const char* m_FragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "\n"
        "uniform sampler2D ourTexture;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(ourTexture, TexCoord);\n"
        "}\n";
};
