#pragma once

#include <cstddef>
#include <vector>

class Texture2DArray
{
	unsigned int _id = 0;

	size_t _width;
	size_t _height;
	size_t _layerCount;
	size_t _mipLevelCount;

public:
	Texture2DArray(size_t w, size_t h, size_t layerCount, size_t mipLevelCount,
	               const std::vector<unsigned char*>& datas);
	~Texture2DArray();
};
