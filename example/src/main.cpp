#include <iostream>

#include <glm/glm.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Clock.hpp>
#include <glgame2d/Input.hpp>
#include <glgame2d/Window.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Renderer.hpp>


int main(void)
{
	Window window{ 640, 480, "title", Window::WindowType::WINDOWED };
	window.enableVSync();

	Shader shader;
	Renderer renderer{ shader, window };

	Camera camera;
	Clock clock{ 60.0f };
	Input input;

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
		input.begin(window);
		if (input.isJustPressed(Mouse::BUTTON_LEFT))
			std::cout << "Pressed\n";
		else if (input.isHeld(Mouse::BUTTON_LEFT))
			std::cout << "Holding\n";
		else if (input.isJustReleased(Mouse::BUTTON_LEFT))
			std::cout << "Released\n";
			
		auto mpos = input.getMousePosition(window);
		std::cout << mpos.xpos << ", " << mpos.ypos << "\n";

		camera.moveEaseTowards(sprite2, window);

		renderer.clear({ 1.0f, 0.0f, 0.0f, 1.0f });
		renderer.beginScene(camera);
		renderer.drawSprite(sprite1);
		renderer.drawSprite(sprite2);

		window.swapBuffers();
		window.pollEvents();

		float deltaTime = clock.tick(false);
		// std::cout << 1.0f / deltaTime << " FPS\n";
	}

	return 0;
}
