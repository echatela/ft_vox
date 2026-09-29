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

/*
*	The noise function should produce values in between [-1, 1]
*/
Texture2D::Texture2D(int res, float f(float x, float y))
    : ATexture(GL_TEXTURE_2D)
{
	noise(res, f);
}

#include <iostream>

void Texture2D::noise(int res, float f(float x, float y))
{
	unsigned char* data = new unsigned char [res * res  * 3];
	unsigned int   i = 0;

	for (int x = 0; x < res; x++)
	{
		for (int y = 0; y < res; y++)
		{
			
			float value = (f(x, y) / 2 + 0.5) * 255;

			if (value > 255/4.f + 30   && value < 255/4.f* 3 - 30)
				value = 0;
			else 
				value = 255;

			// if (value < 125)
			// 	value = 0;
			// else// if (value > 255)
			// 	value = 255;
			
			for (int j = 0; j < 3; j++)
			{

				
		
				// if (value2 > 255/4.f + 30  && value2 < 255/4.f* 3 - 30)
				// 	value = 0;
				
				// std::cout << value << std::endl;

				data[i] = (int)value;
				i++;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	                GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res, res, 0, GL_RGB,
	             GL_UNSIGNED_BYTE, data);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	
	delete [] data;
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
