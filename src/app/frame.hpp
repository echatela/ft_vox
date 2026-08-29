#pragma once

#include <glm/glm.hpp>

struct InputIntent
{
	bool forward = false;
	bool backward = false;
	bool right = false;
	bool left = false;

	bool sprint = false;

	float xOffset = 0.0f;
	float yOffset = 0.0f;
};

struct Frame
{
	double dt = 0.0;

	int width = 0;
	int height = 0;

	InputIntent input;
};
