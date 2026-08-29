#pragma once

#include <string>

class Texture
{
	unsigned int _id;

public:
	Texture(const std::string& path);
	~Texture();

	void bind();
};
