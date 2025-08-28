#pragma once

struct GLFWwindow;


class Window
{
public:
	enum class WindowType {
		WINDOWED,
		WINDOWED_FULLSCREEN,
	};

public:
	Window(int width, int height, const char* title, WindowType type = WindowType::WINDOWED);
	~Window();

	bool shouldClose();
	void swapBuffers();
	void pollEvents() const;

	void enableVSync() const;
	void disableVSync() const;

	void maximize();

	void getSize(int* width, int* height) const;

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_Window;
};
