#include "printer.hpp"

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <algorithm>

Printer::Printer() :
_shader(kShaderVert, kShaderFrag),
_bitmap(kBitmap)
{
	_initLetterInfo();
}

void	Printer::_initLetterInfo()
{
	std::map<const char, letterInfo> map = {

		// min alpha
		{'a', letterInfo(0, 6)},
		{'b', letterInfo(6, 6)},
		{'c', letterInfo(12, 6)},
		{'d', letterInfo(18, 6)},
		{'e', letterInfo(24, 6)},
		{'f', letterInfo(30, 5)},
		{'g', letterInfo(35, 5)},
		{'h', letterInfo(40, 6)},
		{'i', letterInfo(46, 2)},
		{'j', letterInfo(48, 6)},
		{'k', letterInfo(54, 5)},
		{'l', letterInfo(59, 3)},
		{'m', letterInfo(62, 6)},
		{'n', letterInfo(68, 6)},
		{'o', letterInfo(74, 6)},
		{'p', letterInfo(80, 6)},
		{'q', letterInfo(86, 6)},
		{'r', letterInfo(92, 6)},
		{'s', letterInfo(98, 5)},
		{'t', letterInfo(103, 4)},
		{'u', letterInfo(107, 6)},
		{'v', letterInfo(113, 6)},
		{'w', letterInfo(119, 6)},
		{'x', letterInfo(125, 6)},
		{'y', letterInfo(131, 5)},
		{'z', letterInfo(136, 6)},

		// maj alpha
		{'A', letterInfo(142, 6)},
		{'B', letterInfo(148, 6)},
		{'C', letterInfo(154, 6)},
		{'D', letterInfo(160, 6)},
		{'E', letterInfo(166, 6)},
		{'F', letterInfo(172, 6)},
		{'G', letterInfo(178, 6)},
		{'H', letterInfo(184, 6)},
		{'I', letterInfo(190, 4)},
		{'K', letterInfo(194, 6)},
		{'J', letterInfo(200, 6)},
		{'L', letterInfo(206, 6)},
		{'M', letterInfo(212, 6)},
		{'N', letterInfo(218, 6)},
		{'O', letterInfo(224, 6)},
		{'P', letterInfo(230, 6)},
		{'Q', letterInfo(236, 6)},
		{'R', letterInfo(242, 6)},
		{'S', letterInfo(248, 6)},
		{'T', letterInfo(254, 6)},
		{'U', letterInfo(260, 6)},
		{'V', letterInfo(266, 6)},
		{'W', letterInfo(272, 6)},
		{'X', letterInfo(278, 6)},
		{'Y', letterInfo(284, 6)},
		{'Z', letterInfo(290, 6)},

		// digit
		{'0', letterInfo(296, 6)},
		{'1', letterInfo(302, 6)},
		{'2', letterInfo(308, 6)},
		{'3', letterInfo(314, 6)},
		{'4', letterInfo(320, 6)},
		{'5', letterInfo(326, 6)},
		{'6', letterInfo(332, 6)},
		{'7', letterInfo(338, 6)},
		{'8', letterInfo(344, 6)},
		{'9', letterInfo(350, 6)},

		// special char
		{'.', letterInfo(356, 2)},
		{',', letterInfo(358, 2)},
		{';', letterInfo(360, 2)},
		{':', letterInfo(362, 2)},
		{'$', letterInfo(364, 6)},
		{'#', letterInfo(370, 6)},
		{'\'', letterInfo(403, 2)},
		{'!', letterInfo(378, 2)},
		{'"', letterInfo(380, 4)},
		{'/', letterInfo(384, 6)},
		{'?', letterInfo(390, 6)},
		{'%', letterInfo(396, 6)},
		{'&', letterInfo(402, 6)},
		{'(', letterInfo(408, 5)},
		{')', letterInfo(413, 5)},
		{'@', letterInfo(418, 8)},
		{' ', letterInfo(425, 3)}

	};

	_letterMap = map;
}

const LabelMesh Printer::_createLabelMesh(const std::string &text,
	float size, glm::vec2 pos, const glm::ivec2 &resolution) const
{
	LabelMesh       label;

	unsigned int	i;
	letterInfo		info;
	float 			startX, endX, startY, endY;

	std::map<const char, letterInfo>::const_iterator it;

	i = 0;
	size = std::clamp(size, kFontMinSize, kFontMaxSize);

	float height = size / resolution.y * 2;

	startY = -(pos.y / resolution.y * 2 - 1);
	endY = startY - height;

	endX = (pos.x / resolution.x * 2 - 1); // initial xStart

	for (const char letter : text)
	{
		it = _letterMap.find(letter);

		if (it == _letterMap.end())
			it = _letterMap.find('#');

		info = it->second;

		// -----------------------------------------------------

		startX = endX;

		float widthRatio = (info.width) / kFontHeight;

		float xStep = height * widthRatio;

		endX += xStep;

		// -----------------------------------------------------

		// v0
		label.coords.push_back(glm::vec2(startX, startY));
		label.coords.push_back(glm::vec2(info.offset / kFontWidth, 1));

		// v1
		label.coords.push_back(glm::vec2(endX, startY));
		label.coords.push_back(glm::vec2((info.offset + info.width) / kFontWidth, 1));

		// v2
		label.coords.push_back(glm::vec2(endX, endY));
		label.coords.push_back(glm::vec2((info.offset + info.width) / kFontWidth, 0));
		
		// v3
		label.coords.push_back(glm::vec2(startX, endY));
		label.coords.push_back(glm::vec2(info.offset / kFontWidth, 0));

		for (const int vertIndex : {0, 1, 3, 1, 2, 3})
			label.indexes.push_back(i + vertIndex);

		i += 4; // DEFINE
	}

	return (label);
}

//TODO : res devrait pas etre donnee ici
void	Printer::print(const std::string &text, glm::vec2 pos, float size,
						const glm::ivec2 &resolution, const glm::vec3 &color) const
{	

	LabelMesh label = _createLabelMesh(text, size, pos, resolution);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, label.coords.size() * sizeof(glm::vec2), label.coords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, label.indexes.size() * sizeof(unsigned int), label.indexes.data(), GL_STATIC_DRAW);

	// layout 0 -> pos
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)0);
	glEnableVertexAttribArray(0) ;

	// layout 1 -> UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)sizeof(glm::vec2));
	glEnableVertexAttribArray(1);

	_shader.use();
	_bitmap.bind(0);
	_shader.setUniform<int>("myTexture", 0);
	_shader.setUniform<const glm::vec3&>("myColor", color);

	glDrawElements(GL_TRIANGLES, label.indexes.size(), GL_UNSIGNED_INT, (void *)0);
}

Printer::~Printer()
{
}
