#pragma once

#include <string>

class Texture
{
	unsigned int _id;
	bool		 _isLoaded = false;

public:

	void	load(const std::string& path);
	void	bind(unsigned int textureUnit) const;

	Texture();
	Texture(const std::string& path);
	~Texture();

};
