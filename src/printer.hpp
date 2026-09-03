#pragma once

// #include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <map>
#include <stack>

#include "render/shader.hpp"
#include "render/texture.hpp"

static constexpr auto kBitmap = "assets/font/texture_mipmap_font.png";
static constexpr auto kShaderVert = "shaders/2D_vert.glsl";
static constexpr auto kShaderFrag = "shaders/2D_frag.glsl";

static constexpr const float kFontHeight = 8;
static constexpr const float kFontWidth = 500;
static constexpr const float kFontMinSize = 0.001f;
static constexpr const float kFontMaxSize = 1440.0f;

struct letterInfo {

	unsigned int offset;
	unsigned int width;

	letterInfo(unsigned int _offset, unsigned int _width) {
		offset = _offset;
		width = _width;
	}

	letterInfo() {};
};

// struct textInfo {

// 	const std::string 	text;
// 	glm::vec3			color;
// 	float		z;
 
// };

struct LabelMesh {

	std::vector<glm::vec2> 		coords;
	std::vector<unsigned int> 	indexes;

};

class Printer
{
	Shader	_shader;
	Texture _bitmap;

	std::map<const char, letterInfo>	_letterMap;
	std::stack<std::string>				_stack;

	void	        _initLetterInfo();

	const LabelMesh _createLabelMesh(const std::string &text,
		float size, glm::vec2 pos, const glm::ivec2 &resolution) const;

	// void	_push(const std::string textInfo);

public:

	void	print(const std::string &text, glm::vec2 pos, float size, const glm::ivec2 &resolution, const glm::vec3 &color = {0.0, 0.0, 0.0}) const;

	Printer();
	~Printer();

};
