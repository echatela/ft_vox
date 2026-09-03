#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

Window::Window()
{
	GLFWmonitor*       monitor;
	const GLFWvidmode* mode;
	
	monitor = glfwGetPrimaryMonitor();
	if (monitor == nullptr)
		throw std::runtime_error("Failed to get GLFW monitor");

	mode = glfwGetVideoMode(monitor);
	if (mode == nullptr)
		throw std::runtime_error("Failed to get GLFW mode");

	_window = glfwCreateWindow(mode->width, mode->height, "Scop", glfwGetPrimaryMonitor(), NULL);
	if (_window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");
	_resolution = {mode->width, mode->height};

	glfwMakeContextCurrent(_window);

	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
	glfwSetCursorPosCallback(_window, cursorPosCallback);

	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
}

////////////////////////////////////////////////////////////////////////////////

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(_window);
}

void Window::setShouldClose()
{
	glfwSetWindowShouldClose(_window, true);
}

int Window::getWidth() const
{
	return _resolution.x;
}

int Window::getHeight() const
{
	return _resolution.y;
}

const glm::ivec2 &Window::getRes() const
{
	return _resolution;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

bool Window::isKeyPressed(int key)
{
	return glfwGetKey(_window, key) == GLFW_PRESS;
}

void Window::consumeCursorOffset(float* offsetX, float* offsetY)
{
	*offsetX = _cursorOffsetX;
	*offsetY = _cursorOffsetY;
	_cursorOffsetX = 0.0f;
	_cursorOffsetY = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	glfwGetFramebufferSize(window, &width, &height);
	if (self != nullptr)
	{
		self->_resolution.x = width;
		self->_resolution.y = height;
	}
	glViewport(0, 0, width, height);
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (self->_cursorInitialized)
	{
		self->_cursorOffsetX += static_cast<float>(xpos) - self->_cursorLastX;
		self->_cursorOffsetY += self->_cursorLastY - static_cast<float>(ypos);
	}
	self->_cursorLastX = xpos;
	self->_cursorLastY = ypos;
	self->_cursorInitialized = true;
}
