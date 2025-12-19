#include <iostream>

#include <glm/glm.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Clock.hpp>
#include <glgame2d/Entity.hpp>
#include <glgame2d/Input.hpp>
#include <glgame2d/Mixer.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Tilemap.hpp>
#include <glgame2d/Window.hpp>


int main(void)
{
	glgame2d::Window window{ 640, 480, "glgame2d", glgame2d::Window::WINDOWED };
	window.disableVSync();

	glgame2d::Shader shader;
	glgame2d::Renderer renderer{ shader, window };

	glgame2d::Camera camera;
	glgame2d::Clock clock{ 60.0f };
	glgame2d::Input input;

	glgame2d::Entity player{
		glgame2d::Sprite{
			glm::vec2{  0.0f,  0.0f },
			glm::vec2{ 32.0f, 32.0f },
			glgame2d::Texture{ "assets/awesomeface.png" }
		}
	};

	glgame2d::Mixer mixer;
	auto music = mixer.load("assets/898361_The-Disturbance.mp3");
	// mixer.play(music);

	glgame2d::Tilemap tilemap{ "assets/map/map.tmj" };

	float deltaTime = 0.0f;

	while (!window.shouldClose())
	{
		constexpr float PLAYER_MOVE_SPEED = 300.0f;

		glm::vec2 movement{};
		input.begin(window);
		if (input.isHeld(glgame2d::Keyboard::KEY_D))
			movement.x += PLAYER_MOVE_SPEED;
		if (input.isHeld(glgame2d::Keyboard::KEY_A))
			movement.x -= PLAYER_MOVE_SPEED;
		if (input.isHeld(glgame2d::Keyboard::KEY_W))
			movement.y += PLAYER_MOVE_SPEED;
		if (input.isHeld(glgame2d::Keyboard::KEY_S))
			movement.y -= PLAYER_MOVE_SPEED;

		camera.moveEaseTowards(player.getSprite(), window);

		// collision detection
		const auto& physicsTiles = tilemap.physicsSpritesAround(player.getSprite().toRect());
		for (const auto& sprite : physicsTiles)
		{
			if (player.collides(sprite))
				std::cout << "collision\n";
		}

		player.update(deltaTime, tilemap, movement);

		renderer.clear({ 0.0f, 0.0f, 0.0f, 1.0f });
		renderer.beginScene(camera);
		tilemap.render(renderer);
		renderer.drawSprite(player.getSprite());

		window.swapBuffers();
		window.pollEvents();

		deltaTime = clock.tick(true);
		// std::cout << 1.0f / deltaTime << " FPS\n";
	}

	return 0;
}
