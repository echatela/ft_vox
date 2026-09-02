#include "texture_2d_array.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <stdexcept>
#include <string>
#include <vector>
#include <cstddef>

Texture2DArray::Texture2DArray()
    : _width(0),
      _height(0),
      _layerCount(0),
      _internalFormat(GL_RGB),
      _imageFormat(GL_RGB),
      _wrapS(GL_REPEAT),
      _wrapT(GL_REPEAT),
      _filterMin(GL_NEAREST),
      _filterMax(GL_NEAREST)
{
	glGenTextures(1, &_id);
}

Texture2DArray::Texture2DArray(const std::vector<std::string>& texFiles,
                               unsigned int width, unsigned int height,
                               bool alpha)
    : _width(width),
      _height(height),
      _layerCount(texFiles.size()),
      _internalFormat(alpha ? GL_RGBA : GL_RGB),
      _imageFormat(alpha ? GL_RGBA : GL_RGB),
      _wrapS(GL_REPEAT),
      _wrapT(GL_REPEAT),
      _filterMin(GL_NEAREST),
      _filterMax(GL_NEAREST)
{
	glGenTextures(1, &_id);
	loadFromFiles(texFiles, _width, _height, alpha);
}

Texture2DArray::~Texture2DArray()
{
	glDeleteTextures(1, &_id);
}

void Texture2DArray::bind(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
}

////////////////////////////////////////////////////////////////////////////////

void Texture2DArray::loadFromFiles(const std::vector<std::string>& texFiles,
                                   unsigned int width, unsigned int height,
                                   bool alpha)
{
	unsigned char* data;
	int            w, h, n;

	_width = width;
	_height = height;
	_layerCount = texFiles.size();
	if (alpha)
	{
		_internalFormat = GL_RGBA;
		_imageFormat = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, _internalFormat, _width, _height,
	             _layerCount, 0, _imageFormat, GL_UNSIGNED_BYTE, nullptr);

	for (size_t i = 0; i < texFiles.size(); i++)
	{
		data = stbi_load(texFiles.at(i).c_str(), &w, &h, &n, 4);
		if (!data)
		{
			throw std::runtime_error("Texture2DArray: Failed to load image " +
			                         texFiles.at(i));
		}
		if (w != (int)_width || h != (int)_height)
		{
			throw std::runtime_error(
			    "Texture2DArray: Textures must be the same size" +
			    std::to_string(w) + std::to_string(h));
		}
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, _width, _height, 1,
		                _internalFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, _wrapS);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, _wrapT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, _filterMin);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, _filterMax);
	// generate and set mipmap
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
