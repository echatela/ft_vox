
# include <map>
# include "label.hpp"
# include "loader/resource_manager.hpp"


static const std::map<const char, Letter> letterMap = {

		// min alpha
		{'a', Letter(0, 6)},
		{'b', Letter(6, 6)},
		{'c', Letter(12, 6)},
		{'d', Letter(18, 6)},
		{'e', Letter(24, 6)},
		{'f', Letter(30, 5)},
		{'g', Letter(35, 5)},
		{'h', Letter(40, 6)},
		{'i', Letter(46, 2)},
		{'j', Letter(48, 6)},
		{'k', Letter(54, 5)},
		{'l', Letter(59, 3)},
		{'m', Letter(62, 6)},
		{'n', Letter(68, 6)},
		{'o', Letter(74, 6)},
		{'p', Letter(80, 6)},
		{'q', Letter(86, 6)},
		{'r', Letter(92, 6)},
		{'s', Letter(98, 5)},
		{'t', Letter(103, 4)},
		{'u', Letter(107, 6)},
		{'v', Letter(113, 6)},
		{'w', Letter(119, 6)},
		{'x', Letter(125, 6)},
		{'y', Letter(131, 5)},
		{'z', Letter(136, 6)},

		// maj alpha
		{'A', Letter(142, 6)},
		{'B', Letter(148, 6)},
		{'C', Letter(154, 6)},
		{'D', Letter(160, 6)},
		{'E', Letter(166, 6)},
		{'F', Letter(172, 6)},
		{'G', Letter(178, 6)},
		{'H', Letter(184, 6)},
		{'I', Letter(190, 4)},
		{'K', Letter(194, 6)},
		{'J', Letter(200, 6)},
		{'L', Letter(206, 6)},
		{'M', Letter(212, 6)},
		{'N', Letter(218, 6)},
		{'O', Letter(224, 6)},
		{'P', Letter(230, 6)},
		{'Q', Letter(236, 6)},
		{'R', Letter(242, 6)},
		{'S', Letter(248, 6)},
		{'T', Letter(254, 6)},
		{'U', Letter(260, 6)},
		{'V', Letter(266, 6)},
		{'W', Letter(272, 6)},
		{'X', Letter(278, 6)},
		{'Y', Letter(284, 6)},
		{'Z', Letter(290, 6)},

		// digit
		{'0', Letter(296, 6)},
		{'1', Letter(302, 6)},
		{'2', Letter(308, 6)},
		{'3', Letter(314, 6)},
		{'4', Letter(320, 6)},
		{'5', Letter(326, 6)},
		{'6', Letter(332, 6)},
		{'7', Letter(338, 6)},
		{'8', Letter(344, 6)},
		{'9', Letter(350, 6)},

		// special char
		{'.', Letter(356, 2)},
		{',', Letter(358, 2)},
		{';', Letter(360, 2)},
		{':', Letter(362, 2)},
		{'$', Letter(364, 6)},
		{'#', Letter(370, 6)},
		{'\'', Letter(403, 2)},
		{'!', Letter(378, 2)},
		{'"', Letter(380, 4)},
		{'/', Letter(384, 6)},
		{'?', Letter(390, 6)},
		{'%', Letter(396, 6)},
		{'&', Letter(402, 6)},
		{'(', Letter(408, 5)},
		{')', Letter(413, 5)},
		{'@', Letter(418, 8)},
		{' ', Letter(425, 3)}

	};

constexpr const float kFontHeight = 8;
constexpr const float kBitmapWidth = 500;

void Label::_constructMesh()
{
	
	std::vector<glm::vec2> 		coords;
	std::vector<unsigned int> 	indexes;

	unsigned int	i;
	Letter			letter;
	float 			startX, endX, startY, endY;
	float			widthRatio, xStep;

	std::map<const char, Letter>::const_iterator it;

	i = 0;

	endX = 0;
	startY = 0;
	endY = -((int)_size);

	for (const char symbol : _text)
	{
		it = letterMap.find(symbol);

		if (it == letterMap.end())
			it = letterMap.find('#');

		letter = it->second;
		startX = endX;
		widthRatio = letter.width / kFontHeight;
		xStep = _size * widthRatio;
		endX += xStep;

		// v0
		coords.push_back(glm::vec2(startX, startY));
		coords.push_back(glm::vec2(letter.offset / kBitmapWidth, 1));
		// v1
		coords.push_back(glm::vec2(endX, startY));
		coords.push_back(glm::vec2((letter.offset + letter.width) / kBitmapWidth, 1));
		// v2
		coords.push_back(glm::vec2(endX, endY));
		coords.push_back(glm::vec2((letter.offset + letter.width) / kBitmapWidth, 0));
		// v3
		coords.push_back(glm::vec2(startX, endY));
		coords.push_back(glm::vec2(letter.offset / kBitmapWidth, 0));

		for (const int vertIndex : {0, 1, 3, 1, 2, 3})
			indexes.push_back(i + vertIndex);

		i += 4;
	}

	setMesh({coords, indexes});
}

const std::string& Label::getText() const
{
	return _text;
}

void Label::setText(const std::string& text)
{
	_text = text;
	_constructMesh();
}

unsigned int  Label::getSize() const
{
	return _size;
}

void Label::setSize(const unsigned int size)
{
	_size = size;
	_constructMesh();
}

const glm::vec3& Label::getColor() const
{
	return _color;
}

void Label::setColor(const glm::vec3& color)
{
	_color = color;
}

void Label::draw() const
{
	_mesh.bind();
	_material.shader->use();
	_material.shader->setUniform<const glm::vec3&>("myColor", _color);
	Control::draw();
}

Label::Label(std::string text, unsigned int size, glm::vec3 color)
:	_text(text),
	_size(size),
	_color(color)
{
	_constructMesh();
	
	ResourceManager& rm = ResourceManager::instance();
	const Texture* texturePtr = rm.get<Texture>(ResourceId::TEXTURE_FONT);
	const Shader* shaderPtr= rm.get<Shader>(ResourceId::SHADER_CONTROL);
	_material.texture = texturePtr;
	_material.shader = shaderPtr;
}

Label::~Label()
{
}