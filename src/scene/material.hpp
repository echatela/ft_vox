#pragma once

#include "render/shader.hpp"
#include "render/a_texture.hpp"

struct Material
{
	const Shader*   shader = nullptr;
	const ATexture* texture = nullptr;
};
