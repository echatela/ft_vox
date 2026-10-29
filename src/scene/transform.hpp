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
	Anchor       anchor = Anchor::TOP_LEFT;
	glm::vec2	 position = {0, 0};
	glm::vec2	 rect = {0, 0};
	glm::vec2	 scale = {1, 1};
	unsigned int zIndex;

};