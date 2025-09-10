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

	glgame2d::Entity player {
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
		constexpr float PLAYER_MOVE_SENSITIVITY = 120.0f;
		float playerMoveAmount = PLAYER_MOVE_SENSITIVITY * deltaTime;

		input.begin(window);
		if (input.isHeld(glgame2d::Keyboard::KEY_D))
			player.move( playerMoveAmount, 0.0f);
		if (input.isHeld(glgame2d::Keyboard::KEY_A))
			player.move(-playerMoveAmount, 0.0f);
		if (input.isHeld(glgame2d::Keyboard::KEY_W))
			player.move(0.0f,  playerMoveAmount);
		if (input.isHeld(glgame2d::Keyboard::KEY_S))
			player.move(0.0f, -playerMoveAmount);
		
		camera.moveEaseTowards(player.getSprite(), window);

		// collision detection
		const auto& physicsTiles = tilemap.physicsSpritesAround(player);
		for (const auto& sprite : physicsTiles)
		{
			if (player.collides(sprite))
				std::cout << "collision\n";
		}

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
