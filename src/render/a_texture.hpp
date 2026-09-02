#pragma once

#include "loader/resource.hpp"

class ATexture : public Resource
{
protected:
	unsigned int _id = 0;
	unsigned int _target; // GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY...

	explicit ATexture(unsigned int target);

public:
	ATexture(const ATexture&) = delete;
	ATexture& operator=(const ATexture&) = delete;

	virtual ~ATexture();

	void bind(unsigned int textureUnit) const;
};
