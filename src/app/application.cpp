#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app/application.hpp"
#include "app/frame.hpp"

static double deltaTime()
{
	static double last = -1.0;
	double        now = glfwGetTime();
	double        dt = last >= 0.0 ? now - last : 0.0;

	last = now;
	return dt;
}

void Application::run()
{
	Frame frame;

	while (_window.shouldClose() == false)
	{
		_window.pollEvents();

		frame.dt = deltaTime();
		frame.width = _window.getWidth();
		frame.height = _window.getHeight();
		processInput(frame.input);

		_engine.update(frame);
		_engine.render();

		_window.swapBuffers();
	}
}

void Application::processInput(InputIntent& input)
{
	if (_window.isKeyPressed(GLFW_KEY_ESCAPE) == true)
		_window.setShouldClose();

	input.forward = _window.isKeyPressed(GLFW_KEY_W);
	input.backward = _window.isKeyPressed(GLFW_KEY_S);
	input.right = _window.isKeyPressed(GLFW_KEY_D);
	input.left = _window.isKeyPressed(GLFW_KEY_A);
	input.sprint = _window.isKeyPressed(GLFW_KEY_LEFT_SHIFT);

	_window.consumeCursorOffset(&input.xOffset, &input.yOffset);
}
