#include <exception>
#include <iostream>

#include "app/engine.hpp"
#include "platform/glad_context.hpp"
#include "platform/glfw_context.hpp"
#include "platform/window.hpp"

int main()
{
	try
	{
		GlfwContext glfw;
		Window      window;
		GladContext glad;

		Engine engine(window);

		// TODO: engine.init()

		engine.initGUI();
		engine.loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
