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
	while (_window.shouldClose() == false)
	{
		Frame frame;

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

	if (_window.isKeyPressed(GLFW_KEY_W) == true)
		input.forward = true;
	if (_window.isKeyPressed(GLFW_KEY_S) == true)
		input.backward = true;
	if (_window.isKeyPressed(GLFW_KEY_D) == true)
		input.right = true;
	if (_window.isKeyPressed(GLFW_KEY_A) == true)
		input.left = true;
	if (_window.isKeyPressed(GLFW_KEY_LEFT_SHIFT) == true)
		input.sprint = true;

	_window.consumeCursorOffset(&input.xOffset, &input.yOffset);
}
