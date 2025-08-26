#include <glm/glm.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Window.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Renderer.hpp>


int main(void)
{
	Window window{ 640, 480, "title" };

	Shader shader;
	Renderer renderer{ shader, window };

	Camera camera;

	Sprite sprite1 {
		glm::vec2{ 1.0f, 0.0f },
		glm::vec2{ 64.0f, 64.0f },
		Texture{ "assets/container.jpg" }
	};

	Sprite sprite2 {
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ 64.0f, 64.0f },
		Texture{ "assets/awesomeface.png" }
	};

	while (!window.shouldClose())
	{
		camera.move(-0.001f, -0.001f);

		renderer.clear();

		renderer.beginScene(camera);
		renderer.drawSprite(sprite1, shader);
		renderer.drawSprite(sprite2, shader);

		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}
