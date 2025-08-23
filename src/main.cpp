#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Window.hpp"
#include "QuadVAO.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"


int main(void)
{
	Window window(640, 480, "title");

	QuadVAO vao;
	Shader shader;
	Renderer renderer(shader, window);

	Sprite sprite1{
		glm::vec2(1.0f, 0.0f),
		glm::vec2(64.0f, 64.0f),
		Texture{ "assets/container.jpg" }
	};

	Sprite sprite2{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(64.0f, 64.0f),
		Texture{ "assets/awesomeface.png" }
	};

	while (!window.shouldClose())
	{
		renderer.clear();

		renderer.drawSprite(sprite1, shader, vao);
		renderer.drawSprite(sprite2, shader, vao);

		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}

