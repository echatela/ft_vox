#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	glfwGetFramebufferSize(window, &width, &height);
	if (self != nullptr)
	{
		self->_width = width;
		self->_height = height;
	}
	glViewport(0, 0, width, height);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)xoffset;
	Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (self != nullptr)
		self->_scrollOffset += yoffset;
}

Window::Window()
{
	_window = glfwCreateWindow(800, 600, "Scop", NULL, NULL);
	if (_window == nullptr)
		throw std::runtime_error("Failed to create GLFW window");

	glfwMakeContextCurrent(_window);

	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);
	glfwSetScrollCallback(_window, scrollCallback);
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

void Window::getFramebufferSize(int* width, int* height) const
{
	*width = _width;
	*height = _height;
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

double Window::consumeScroll()
{
	double s = _scrollOffset;
	_scrollOffset = 0.0;
	return s;
}
