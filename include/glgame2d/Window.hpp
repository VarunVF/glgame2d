#pragma once

#include <string>


struct GLFWwindow;


namespace glgame2d {


class Window
{
public:
	enum WindowType {
		WINDOWED,
		WINDOWED_FULLSCREEN,
	};

	struct WindowSize {
		int width;
		int height;
	};

	struct FramebufferSize {
		int width;
		int height;
	};

public:
	Window(int width, int height, const char* title, WindowType type = WindowType::WINDOWED, bool hideWindow = false);
	~Window();

	bool shouldClose() const;
	void swapBuffers();
	void pollEvents();

	void enableVSync();
	void disableVSync();
	
	void show();
	void hide();
	void maximize();

	void setIcon(const std::string& iconPaths);

	WindowSize getWindowSize() const;
	FramebufferSize getFramebufferSize() const;
	GLFWwindow* getGLFWWindow() const;

private:
	static void screenSizeCallback(GLFWwindow* window, int width, int height);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_Window;
	int m_WindowWidth;
	int m_WindowHeight;
	int m_FramebufferWidth;
	int m_FramebufferHeight;
};


} // namespace glgame2d
