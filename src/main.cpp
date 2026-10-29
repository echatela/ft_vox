#include <exception>
#include <iostream>

#include "loader/resource_manager.hpp"
#include "app/engine.hpp"
#include "platform/glad_context.hpp"
#include "platform/glfw_context.hpp"
#include "platform/window.hpp"

#include "time.hpp"

int main()
{
	try
	{
		timeinfo::startTimer();		

		GlfwContext glfw;
		Window      window;
		GladContext glad;
		ResourceManagerGuard rmGuard;


		Engine engine(window);
		
		engine.init();

		timeinfo::stopTimer();
		std::cout << "Init time is : " << timeinfo::timeStr() << std::endl;

		// exit(0);
		engine.loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
