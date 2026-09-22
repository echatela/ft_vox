#pragma once

#include <glm/vec2.hpp>

struct ControlTransform {

	// unsigned int rect[4] = {};
	glm::vec2	 position;
	glm::vec2	 rect;
	glm::vec2	 scale;
	unsigned int zIndex;

};