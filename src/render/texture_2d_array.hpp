#pragma once

#include "render/a_texture.hpp"

#include <string>
#include <vector>

class Texture2DArray : public ATexture
{
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

<<<<<<< HEAD
	void load(const std::vector<std::string>& texFiles, unsigned int width,
	          unsigned int height, bool alpha);
=======
	void bind(unsigned int textureUnit = 0) const;

	void loadFromFiles(const std::vector<std::string>& texFiles,
	                   unsigned int width, unsigned int height, bool alpha);

	void         setInternalFormat(unsigned int format);
	unsigned int getInternalFormat() const;
	void         setImageFormat(unsigned int format);
	unsigned int getImageFormat() const;

	void         setWrapS(unsigned int wrap);
	unsigned int getWrapS() const;
	void         setWrapT(unsigned int wrap);
	unsigned int getWrapT() const;
	void         setFilterMin(unsigned int filter);
	unsigned int getFilterMin() const;
	void         setFilterMax(unsigned int filter);
	unsigned int getFilterMax() const;
>>>>>>> df903ab (feat: mipmaps, setters and getters)
};
