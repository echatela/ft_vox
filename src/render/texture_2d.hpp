#pragma once

#include "render/a_texture.hpp"

#include <string>

class Texture2D : public ATexture
{
public:
	Texture2D();
	Texture2D(const std::string& path);
	Texture2D(int res, float f(float x, float y)); // noise constructor


	void noise(int res, float f(float x, float y)); //build texture from noise function
	void load(const std::string& path);
};
