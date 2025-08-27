#include <iostream>

#include <glm/glm.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Clock.hpp>
#include <glgame2d/Window.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Renderer.hpp>


int main(void)
{
	Window window{ 640, 480, "title" };
	window.disableVSync();

	Shader shader;
	Renderer renderer{ shader, window };

	Camera camera;
	Clock clock{ 120.0f };

	Sprite sprite1 {
		glm::vec2{ 0.0f, 0.0f },
		glm::vec2{ 64.0f, 64.0f },
		Texture{ "assets/container.jpg" }
	};

	Sprite sprite2 {
		glm::vec2{ 64.0f, 0.0f },	
		glm::vec2{ 64.0f, 64.0f },
		Texture{ "assets/awesomeface.png" }
	};

	while (!window.shouldClose())
	{
		renderer.clear();

		renderer.beginScene(camera);
		renderer.drawSprite(sprite1);
		renderer.drawSprite(sprite2);

		window.swapBuffers();
		window.pollEvents();

		float deltaTime = clock.tick(true);
		std::cout << 1.0f / deltaTime << " FPS\n";
	}

	return 0;
}
