#pragma once
#include <string>

namespace timeinfo
{

	void	startTimer();
	void	stopTimer();
	std::string timeStr();
	double	deltaTime();
	int   	getFramerate(double delta);

} // namespace timeinfo
