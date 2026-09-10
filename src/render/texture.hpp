#pragma once

#include <string>
#include "loader/resource.hpp"

class Texture : public Resource
{
	unsigned int _id;
	bool		 _isLoaded = false;

public:

	void	load(const std::string& path);
	void	bind(unsigned int textureUnit) const;

	Texture();
	Texture(const char* path);
	~Texture();

};
