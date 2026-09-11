#include "render/a_texture.hpp"
#include "render/shader.hpp"

ATexture::ATexture(unsigned int target)
    : _target(target)
{
	glGenTextures(1, &_id);
}

ATexture::~ATexture()
{
	glDeleteTextures(1, &_id);
	_id = 0;
}

void ATexture::bind(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(_target, _id);
}
