#include <glgame2d/Font.hpp>
#include <glgame2d/FontRenderer.hpp>
#include <glgame2d/Window.hpp>
#include <glgame2d/Renderer.hpp>


int main()
{
    glgame2d::Window window{ 800, 600, "Font rendering example" };
    glgame2d::Renderer renderer{};
    glgame2d::FontRenderer fontRenderer{};
    glgame2d::Font arial{ "assets/arial.ttf", 64.0f };
    glgame2d::Font consola{ "assets/consola.ttf", 64.0f };

    window.enableVSync();

    while (!window.shouldClose())
    {
        renderer.clear({ 0.2f, 0.2f, 0.2f, 1.0f });
        
        fontRenderer.drawBegin();
        fontRenderer.drawText(window, arial, "Font rendering example", { -0.8f, 0.4f, 0.0f }, { 0.9f, 0.2f, 0.3f, 1.0f }, 1.0f);
        fontRenderer.drawText(window, arial, "This is some text rendered in OpenGL.", { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
        fontRenderer.drawText(window, arial, "The color of text can be changed too!", { -0.5f, -0.4f, 0.0f }, { 0.1f, 0.5f, 1.0f, 1.0f }, 0.5f);
        fontRenderer.drawText(window, arial, "This is a \nNewline demo", { -0.9f, -0.7f, 0.0f }, { 0.7f, 0.8f, 0.0f, 1.0f }, 0.6f);
        fontRenderer.renderFrame(arial);

        fontRenderer.drawBegin();
        fontRenderer.drawText(window, consola, "Here's another font", { 0.0f, -0.7f, 0.0f }, { 0.8f, 0.5f, 0.4f, 1.0f }, 0.4f);
        fontRenderer.renderFrame(consola);

        window.swapBuffers();
        window.pollEvents();
    }
}
