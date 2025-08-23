#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"


int main(void)
{
	Window window(640, 480, "title");

	while (!window.shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}

