#pragma once

#include "math/vec3.hpp"
#include "math/vec2.hpp"

struct InputIntent
{
	scm::Vec3 move;
	bool      sprint = false;

	scm::Vec2 look;
};

struct Frame
{
	double dt = 0.0;

	int width = 0;
	int height = 0;

	InputIntent input;
};
