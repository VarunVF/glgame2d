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

	void getSize(int* width, int* height) const;
	GLFWwindow* getGLFWWindow() const;

private:
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_Window;
};


} // namespace glgame2d
