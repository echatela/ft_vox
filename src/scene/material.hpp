#pragma once

#include "render/shader.hpp"
#include "render/texture.hpp"

struct Material {
	const Shader  *shader = nullptr;
	const Texture *texture = nullptr;
};