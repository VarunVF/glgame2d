#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(void)
{
	if (!glfwInit())
		return -1;
	
	// Load modern OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 640, height = 480;
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialise glad before making OpenGL calls
	if (gladLoadGL() == 0)
	{
		std::cerr << "Failed to initialise OpenGL context\n";
		glfwTerminate();
		return -1;
	}
	std::clog << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	// Tell OpenGL where it can draw
	glViewport(0, 0, width, height);

	// Update viewport on window resize
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

