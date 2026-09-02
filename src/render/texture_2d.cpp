#include "render/texture_2d.hpp"
#include "render/a_texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fcntl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

Texture2D::Texture2D()
    : ATexture(GL_TEXTURE_2D)
{
}

Texture2D::Texture2D(const std::string& path)
    : ATexture(GL_TEXTURE_2D)
{
	load(path);
}

void Texture2D::load(const std::string& path)
{
	unsigned char* data;
	int            width, height, nrChannels;

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
}
