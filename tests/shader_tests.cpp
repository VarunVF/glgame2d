#include <catch2/catch_test_macros.hpp>

#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>


TEST_CASE("Shader can compile correctly", "[Shader]")
{
    glgame2d::Window window{ 640, 480, "title" };
    glgame2d::Renderer renderer{};

    SECTION("Renderer shader pair compiles without crash")
    {
        glgame2d::Renderer renderer{};
        REQUIRE( true );
    }

    SECTION("Shader source with errors does not crash")
    {
        const char* vertexSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec2 a_Pos;\n"
            "layout (location = 1) in vec2 a_TexCoord;\n"
            "\n"    // missing uniform declarations
            "void main()\n"
            "{\n"
            "    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 0.0, 1.0);\n"
            "    TexCoord = a_TexCoord;\n"
            "}\n";

        const char* fragmentSource =
            "#version 330 core\n"
            "out vec4 FragColor\n"  // missing semicolon
            "\n"
            "in vec2 TexCoord;\n"
            "\n"
            "uniform sampler2D u_Texture;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = texture(u_Texture, TexCoord);\n"
            "}\n";
        
        glgame2d::Shader shader{ vertexSource, fragmentSource };
        REQUIRE( shader.shaderProgram == 0 );
    }
    
    SECTION("Shader can compile from external files")
    {
        auto shader = glgame2d::Shader::fromFiles("shaders/test.vsh", "shaders/test.fsh");

        REQUIRE(shader.shaderProgram != 0);
    }

    SECTION("Shader compilation from non-existent files does not crash")
    {
        auto shader = glgame2d::Shader::fromFiles("no/such/file.vsh", "no/such/file.fsh");

        REQUIRE(shader.shaderProgram == 0);
    }
}
