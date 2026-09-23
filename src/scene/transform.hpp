#pragma once

#include <glm/vec2.hpp>

enum class Anchor {
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	CENTER_LEFT,
	CENTER,
	CENTER_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT
};

struct ControlTransform {

	// unsigned int rect[4] = {};
	Anchor       anchor;
	glm::vec2	 position;
	glm::vec2	 rect;
	glm::vec2	 scale;
	unsigned int zIndex;

};