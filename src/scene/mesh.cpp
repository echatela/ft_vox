#include "mesh.hpp"

#include "glad/glad.h"
#include <iostream>

void Mesh2d::generateGPUBuffers()
{
	if (_activeBuffers)
	{
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_EBO);
	}
	_activeBuffers = true;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _coords.size() * sizeof(glm::vec2), _coords.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexes.size() * sizeof(unsigned int), _indexes.data(), GL_STATIC_DRAW);

	// layout 0 -> pos
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)0);
	glEnableVertexAttribArray(0) ;
	// layout 1 -> UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)sizeof(glm::vec2));
	glEnableVertexAttribArray(1);
}

const std::vector<glm::vec2>& Mesh2d::getCoords() const
{
	return _coords;
}

const std::vector<unsigned int>& Mesh2d::getIndexes() const
{
	return _indexes;
}

unsigned int Mesh2d::getVAO() const
{
	return (_VAO);
}

// TODO : not sure about this function
// I need it to be able to update the mesh
// but I'm afraid there might be problems with VAO deletion etc
void	Mesh2d::operator=(const Mesh2d &other)
{
	if (this == &other)
		return ;

	glDeleteVertexArrays(1, &_VAO);
	if (_activeBuffers)
	{
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_EBO);
	}
	
	_coords = other._coords;
	_indexes = other._indexes;
	glGenVertexArrays(1, &_VAO);
	generateGPUBuffers();
}

Mesh2d::Mesh2d(std::vector<glm::vec2> coords, std::vector<unsigned int> indexes)
: _coords(coords),
  _indexes(indexes)
{
	glGenVertexArrays(1, &_VAO);
	generateGPUBuffers();
}

Mesh2d::Mesh2d()
{
	glGenVertexArrays(1, &_VAO);
}

Mesh2d::~Mesh2d()
{
	glDeleteVertexArrays(1, &_VAO);
	if (_activeBuffers)
	{
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_EBO);
	}
}