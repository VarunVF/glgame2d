#include "glgame2d/Window.hpp"

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


Window::Window(int width, int height, const char* title)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initalise GLFW");
	
	// Load modern OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_Window)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window);

	// Initialise glad before making OpenGL calls
	if (gladLoadGL() == 0)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		throw std::runtime_error("Failed to initialise OpenGL context");
	}
	std::clog << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

	// Tell OpenGL where it can draw
	glViewport(0, 0, width, height);

	// Update viewport on window resize
	glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_Window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::getSize(int *width, int *height) const
{
    glfwGetWindowSize(m_Window, width, height);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
