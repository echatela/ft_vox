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
		_window.getFramebufferSize(&frame.width, &frame.height);
		processInput(frame.input);

//		_engine.update(frame);
//		_engine.render();

		_window.swapBuffers();
	}
}

void Application::processInput(InputIntent& input)
{
	if (_window.isKeyPressed(GLFW_KEY_ESCAPE) == true)
		_window.setShouldClose();

	if (_window.isKeyPressed(GLFW_KEY_W) == true)
		input.move.z--;
	if (_window.isKeyPressed(GLFW_KEY_S) == true)
		input.move.z++;
	if (_window.isKeyPressed(GLFW_KEY_A) == true)
		input.move.x--;
	if (_window.isKeyPressed(GLFW_KEY_D) == true)
		input.move.x++;
	if (_window.isKeyPressed(GLFW_KEY_SPACE) == true)
		input.move.y++;
	if (_window.isKeyPressed(GLFW_KEY_LEFT_SHIFT) == true)
		input.move.y--;

	_window.consumeCursorOffset(&input.look.x, &input.look.y);
}
