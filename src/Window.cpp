#include "glgame2d/Window.hpp"
#include "glgame2d/GLCall.hpp"

#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace glgame2d {


Window::Window(int width, int height, const char* title, WindowType type)
{
	if (!glfwInit())
		throw std::runtime_error("[Window] Failed to initalise GLFW");
	
	// Load modern OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Get monitor resolution for fullscreen window
	GLFWmonitor* monitor = nullptr;
	const GLFWvidmode* mode = nullptr;
	int viewportWidth = 0, viewportHeight = 0;
	if (type == WindowType::WINDOWED)
	{
		viewportWidth = width;
		viewportHeight = height;
	}
	else if (type == WindowType::WINDOWED_FULLSCREEN)
	{
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		viewportWidth = mode->width;
		viewportHeight = mode->height;
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	m_Window = glfwCreateWindow(viewportWidth, viewportHeight, title, monitor, NULL);
	if (!m_Window)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		throw std::runtime_error("[Window] Failed to create GLFW window");
	}

	glfwMakeContextCurrent(m_Window);

	// Initialise glad before making OpenGL calls
	if (gladLoadGL() == 0)
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		throw std::runtime_error("[Window] Failed to initialise OpenGL context");
	}
	std::clog << "[Window] OpenGL version: " << glGetString(GL_VERSION) << "\n";

	// Tell OpenGL where it can draw
	GLCall( glViewport(0, 0, viewportWidth, viewportHeight) );

	// Update viewport on window resize
	glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

	// Avoid screen tearing in fullscreen modes
	if (type == WindowType::WINDOWED_FULLSCREEN)
		enableVSync();
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

void Window::pollEvents() const
{
	glfwPollEvents();
}

void Window::enableVSync() const
{
	glfwSwapInterval(1);
}

void Window::disableVSync() const
{
	glfwSwapInterval(0);
}

void Window::maximize()
{
	glfwMaximizeWindow(m_Window);
}

void Window::getSize(int *width, int *height) const
{
    glfwGetWindowSize(m_Window, width, height);
}

GLFWwindow *Window::getGLFWWindow() const
{
    return m_Window;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GLCall( glViewport(0, 0, width, height) );
}


} // namespace glgame2d
