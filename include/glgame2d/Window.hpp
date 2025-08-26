#pragma once

struct GLFWwindow;


class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	bool shouldClose();
	void swapBuffers();
	void pollEvents() const;

	void enableVSync() const;
	void disableVSync() const;

	void getSize(int* width, int* height) const;

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_Window;
};
