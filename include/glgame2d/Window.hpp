#pragma once

struct GLFWwindow;


namespace glgame2d {


class Window
{
public:
	enum WindowType {
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
	GLFWwindow* getGLFWWindow() const;

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_Window;
};


} // namespace glgame2d
