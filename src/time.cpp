
#include "time.hpp"
#include "GLFW/glfw3.h"

namespace timeinfo
{
static constexpr const unsigned int kFramerateRange = 32;

static double timerStart = 0;
static double timerStop = 0;
static double timer = 0;

void startTimer()
{
	timerStart = glfwGetTime();
}

void stopTimer()
{
	timerStop = glfwGetTime();
	timer = timerStop - timerStart;
}

std::string timeStr()
{
	std::string result = std::to_string(timer);
	result.resize(4);
	return (result);
}
double deltaTime()
{
	static double last = -1.0;
	double        now = glfwGetTime();
	double        dt = last >= 0.0 ? now - last : 0.0;

	last = now;
	return dt;
}

/*
This function should be called only once a frame !
*/
int getFramerate(double delta)
{
	static double       deltas[kFramerateRange] = {};
	static unsigned int i = 0;
	static float        average = 0;

	average += delta - deltas[i];
	deltas[i] = delta;
	i = (i + 1) % kFramerateRange;

	return (kFramerateRange / average);
}
} // namespace timeinfo
