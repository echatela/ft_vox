#include "texture_2d_array.hpp"

#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Texture2DArray::Texture2DArray(size_t w, size_t h, size_t layerCount,
                               size_t                             mipLevelCount,
                               const std::vector<unsigned char*>& datas)
    : _width(w),
      _height(h),
      _layerCount(layerCount),
      _mipLevelCount(mipLevelCount)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, w, h, datas.size(), 0,
	             GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	for (size_t i = 0; i < datas.size(); i++)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, w, h, 1, GL_RGBA,
		                GL_UNSIGNED_BYTE, datas.at(0));
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// generate and set mipmap
}
