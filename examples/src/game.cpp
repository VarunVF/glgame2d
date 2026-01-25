#include <iostream>
#include <format>

#include <glm/glm.hpp>

#include <glgame2d/Camera.hpp>
#include <glgame2d/Clock.hpp>
#include <glgame2d/Entity.hpp>
#include <glgame2d/Font.hpp>
#include <glgame2d/Input.hpp>
#include <glgame2d/Mixer.hpp>
#include <glgame2d/Renderer.hpp>
#include <glgame2d/Shader.hpp>
#include <glgame2d/Tilemap.hpp>
#include <glgame2d/Window.hpp>


int main(void)
{
	// A Window is required before creating other objects (such as Renderer, Texture)
	glgame2d::Window window{ 640, 480, "glgame2d", glgame2d::Window::WINDOWED };
	window.enableVSync();
	
	glgame2d::Renderer renderer{};

	glgame2d::Camera camera{};
	glgame2d::Input input{};
	
	glgame2d::Font font{ "assets/arial.ttf", 64.0f };

	glgame2d::Entity player{
		glgame2d::Sprite{
			glm::vec2{  0.0f,  0.0f },
			glm::vec2{ 32.0f, 32.0f },
			glgame2d::Texture{ "assets/awesomeface.png" }
		}
	};

	//glgame2d::Mixer mixer;
	//auto music = mixer.load("assets/898361_The-Disturbance.mp3");
	//mixer.play(music);

	glgame2d::Tilemap tilemap{ "assets/map/map.tmj" };
	
	// The clock should start ticking right before the mainloop,
	// so that deltaTime excludes the initial loading.
	float deltaTime = 0.0f;
	glgame2d::Clock clock{ 60.0f };

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

		player.update(deltaTime, tilemap, movement);

		renderer.clear({ 0.0f, 0.0f, 0.0f, 1.0f });
		renderer.beginScene(camera);
		tilemap.render(renderer, window);
		renderer.drawSprite(window, player.getSprite());

		font.drawBegin();
		std::string fpsString = std::format("{:.1f} FPS  ", 1.0 / deltaTime);
		font.drawText(window, fpsString, { 0.8f, -0.8f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.5f }, 0.5f);
		font.renderFrame();

		window.swapBuffers();
		window.pollEvents();

		deltaTime = clock.tick(false);  // Whether to cap the framerate
	}

	return 0;
}
