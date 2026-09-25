#pragma once

#include <glm/glm.hpp>

struct InputIntent
{
	bool numpad1 = false;
	bool numpad2 = false;
	bool numpad3 = false;
	bool numpad4 = false;
	bool numpad5 = false;
	bool numpad6 = false;
	bool numpad7 = false;
	bool numpad8 = false;
	bool numpad9 = false;

	bool forward = false;
	bool backward = false;
	bool right = false;
	bool left = false;
	bool up = false;

	bool sprint = false;
	bool infoKeyHold = false;
	bool infoToggle = false;

	float xOffset = 0.0f;
	float yOffset = 0.0f;
};

struct Frame
{
	double dt = 0.0;

	glm::ivec2 resolution = {0, 0};

	InputIntent input;
};
