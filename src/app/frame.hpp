#pragma once

#include <glm/glm.hpp>

struct InputIntent
{
	glm::ivec3 move;
	bool sprint = false;

	glm::vec2 look;
};

struct Frame
{
	double dt = 0.0;

	int width = 0;
	int height = 0;

	InputIntent input;
};
