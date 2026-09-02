#pragma once

#include "render/a_texture.hpp"

#include <string>

class Texture2D : public ATexture
{
public:
	Texture2D();
	Texture2D(const std::string& path);

	void load(const std::string& path);
};
