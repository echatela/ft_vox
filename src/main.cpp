#include <exception>
#include <iostream>

#include "loader/resource_manager.hpp"
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
		ResourceManagerGuard rmGuard;

		Engine engine(window);
		
		engine.init();
		engine.loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
