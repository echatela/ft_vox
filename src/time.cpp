
#include "time.hpp"
#include "GLFW/glfw3.h"

namespace timeinfo {

	static constexpr const unsigned int kFramerateRange = 32;

	double deltaTime()
	{
		static double last = -1.0;
		double        now = glfwGetTime();
		double        dt = last >= 0.0 ? now - last : 0.0;

		last = now;
		return dt;
	}

	int getFramerate(double delta)
	{
		static double       deltas[kFramerateRange] = {};
		static unsigned int i = 0;
		static float		average = 0;

		average += delta - deltas[i];
		i = (i + 1) % kFramerateRange;
		deltas[i] = delta;

		return (kFramerateRange / average);
	}
	
}