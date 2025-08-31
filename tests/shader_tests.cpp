#include <catch2/catch_test_macros.hpp>

#include <glgame2d/QuadVAO.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>


TEST_CASE("Sprite compilation does not cause crashes", "[Sprite]")
{
    glgame2d::Window window{ 640, 480, "title" };
	glgame2d::QuadVAO vao;
	glgame2d::Shader shader;
	glgame2d::Renderer renderer{ shader, window };

    SECTION("Default shader pair compiles without crash")
    {
        glgame2d::Shader defaultShader;
        REQUIRE( true );
    }

    SECTION("Shader source with errors does not crash")
    {
        const char* vertexSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec2 aTexCoord;\n"
            "\n"    // missing uniform declarations
            "void main()\n"
            "{\n"
            "    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 0.0, 1.0);\n"
            "    TexCoord = aTexCoord;\n"
            "}\n";

        const char* fragmentSource =
            "#version 330 core\n"
            "out vec4 FragColor\n"  // missing semicolon
            "\n"
            "in vec2 TexCoord;\n"
            "\n"
            "uniform sampler2D ourTexture;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = texture(ourTexture, TexCoord);\n"
            "}\n";
        
        glgame2d::Shader shader{ vertexSource, fragmentSource };
        REQUIRE( true );
    }
}
