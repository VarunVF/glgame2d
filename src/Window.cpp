#include "glgame2d/Window.hpp"
#include "glgame2d/GLCall.hpp"
#include "glgame2d/logging.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

#include "glgame2d/OpenGL.hpp"
#include <GLFW/glfw3.h>
#include <stb_image.h>


namespace glgame2d {


Window::Window(int width, int height, const char* title, WindowType type, bool hideWindow)
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

    if (hideWindow)
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(viewportWidth, viewportHeight, title, monitor, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        throw std::runtime_error("[Window] Failed to create GLFW window");
    }
    // Callbacks need the instance, must set before registering callbacks
    glfwSetWindowUserPointer(m_Window, this);

    // Cache the values after window creation
    glfwGetWindowSize(m_Window, &m_WindowWidth, &m_WindowHeight);
    glfwGetFramebufferSize(m_Window, &m_FramebufferWidth, &m_FramebufferHeight);
    
    glfwMakeContextCurrent(m_Window);

    // Initialise glad before making OpenGL calls. On the web the WebGL
    // functions are linked directly and there is nothing to load.
#ifndef __EMSCRIPTEN__
    if (gladLoadGL() == 0)
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        throw std::runtime_error("[Window] Failed to initialise OpenGL context");
    }
#endif
    logging::info("[Window] OpenGL version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    float xscale = 0.0f, yscale = 0.0f;
    glfwGetWindowContentScale(m_Window, &xscale, &yscale);
    glgame2d::logging::info("[Window] Window size: {}x{}, Framebuffer size: {}x{}, Content scale: {}x{}",
        m_WindowWidth, m_WindowHeight, m_FramebufferWidth, m_FramebufferHeight, xscale, yscale);

    // Tell OpenGL where it can draw
    GLCall(glViewport(0, 0, m_FramebufferWidth, m_FramebufferHeight));

    // Update dimensions and viewport on resize
    glfwSetWindowSizeCallback(m_Window, screenSizeCallback);
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

    // Avoid screen tearing in fullscreen modes
    if (type == WindowType::WINDOWED_FULLSCREEN)
        enableVSync();
}

Window::~Window()
{
    glfwSetWindowSizeCallback(m_Window, nullptr);
    glfwSetFramebufferSizeCallback(m_Window, nullptr);
    glfwSetWindowUserPointer(m_Window, nullptr);

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::shouldClose() const
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

void Window::enableVSync()
{
    glfwSwapInterval(1);
}

void Window::disableVSync()
{
    glfwSwapInterval(0);
}

void Window::show()
{
    glfwShowWindow(m_Window);
}

void Window::hide()
{
    glfwHideWindow(m_Window);
}

void Window::maximize()
{
    glfwMaximizeWindow(m_Window);
}

void Window::setIcon(const std::string& iconPath)
{
    int nrChannels;
    GLFWimage image;
    
    // GLFW requires RGBA format, so force 4 channels per pixel
    image.pixels = stbi_load(iconPath.c_str(), &image.width, &image.height, &nrChannels, 4);
    if (image.pixels) {
        glfwSetWindowIcon(m_Window, 1, &image);
        stbi_image_free(image.pixels);
    }
    else {
        logging::warn("[Window] Failed to load icon image '{}'", iconPath);
    }
}

Window::WindowSize Window::getWindowSize() const
{
    return { m_WindowWidth, m_WindowHeight };
}

Window::FramebufferSize Window::getFramebufferSize() const
{
    return { m_FramebufferWidth, m_FramebufferHeight };
}

GLFWwindow *Window::getGLFWWindow() const
{
    return m_Window;
}

void Window::screenSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    assert(instance != nullptr);
    instance->m_WindowWidth = width;
    instance->m_WindowHeight = height;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    assert(instance != nullptr);
    instance->m_FramebufferWidth = width;
    instance->m_FramebufferHeight = height;

    GLCall(glViewport(0, 0, width, height));
}


} // namespace glgame2d
