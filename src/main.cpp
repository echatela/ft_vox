#include <exception>
#include <iostream>

#include "app/engine.hpp"

int main()
{
	try
	{
		Engine engine;

		engine.loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
