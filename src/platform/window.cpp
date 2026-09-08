#include "window.hpp"

#include <array>
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

	_window = glfwCreateWindow(mode->width, mode->height, "Scop",
	                           glfwGetPrimaryMonitor(), NULL);
	if (_window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");
	_resolution = {mode->width, mode->height};

	glfwMakeContextCurrent(_window);

	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, cursor_pos_callback);
	glfwSetKeyCallback(_window, key_callback);

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

const glm::ivec2& Window::getRes() const
{
	return _resolution;
}

const std::array<bool, 1024>& Window::getKeys() const
{
	return _keys;
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

void Window::framebuffer_size_callback(GLFWwindow* window, int width,
                                       int height)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (self != nullptr)
	{
		self->_resolution.x = width;
		self->_resolution.y = height;
	}
	glViewport(0, 0, width, height);
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (self)
	{
		self->_cursorOffsetX += static_cast<float>(xpos) - self->_cursorLastX;
		self->_cursorOffsetY += self->_cursorLastY - static_cast<float>(ypos);
		self->_cursorLastX = xpos;
		self->_cursorLastY = ypos;
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action,
                          int mods)
{
	(void)scancode;
	(void)mods;
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (self)
	{
		if (action == GLFW_PRESS)
			self->_keys[key] = true;
		if (action == GLFW_RELEASE)
			self->_keys[key] = false;
	}
}
