#pragma once

#include <glm/glm.hpp>

struct InputIntent
{
	glm::vec3 move{0.0f};
	bool      moved = false;
	bool      sprint = false;

	glm::vec2 look{0.0f};
};

struct Frame
{
	double dt = 0.0;

	int width = 0;
	int height = 0;

	InputIntent input;
};
