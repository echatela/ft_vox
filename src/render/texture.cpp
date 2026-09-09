#include "render/texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fcntl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

void	Texture::load(const std::string& path)
{
	if (_isLoaded)
		glDeleteTextures(1, &_id);

	unsigned char* data;
	int width, height, nrChannels;

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	                GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
	if (!data)
		throw std::runtime_error("texture: Failed to load image");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	_isLoaded = true;
}

Texture::Texture()
{
}

Texture::Texture(const std::string& path)
{
	load(path);
}

void Texture::bind(unsigned int textureUnit) const
{
	if (!_isLoaded)
		return ;
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _id);
}

Texture::~Texture()
{
	if (_isLoaded)
		glDeleteTextures(1, &_id);
}
