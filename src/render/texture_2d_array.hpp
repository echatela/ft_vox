#pragma once

#include <string>
#include <vector>

class Texture2DArray
{
	unsigned int _id;

	unsigned int _width;
	unsigned int _height;
	unsigned int _layerCount;

	unsigned int _internalFormat;
	unsigned int _imageFormat;

	unsigned int _wrapS;
	unsigned int _wrapT;
	unsigned int _filterMin;
	unsigned int _filterMax;

public:
	Texture2DArray();
	Texture2DArray(const std::vector<std::string>& texFiles, unsigned int width,
	               unsigned int height, bool alpha);
	~Texture2DArray();

	void bind(unsigned int textureUnit = 0) const;

	void loadFromFiles(const std::vector<std::string>& texFiles,
	                   unsigned int width, unsigned int height, bool alpha);
};
