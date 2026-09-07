#include "mesh.hpp"

const std::vector<glm::vec2>& Mesh2d::getCoords() const
{
	return _coords;
}

const std::vector<unsigned int>& Mesh2d::getIndexes() const
{
	return _indexes;
}


Mesh2d::Mesh2d(std::vector<glm::vec2> coords, std::vector<unsigned int> indexes)
: _coords(coords),
  _indexes(indexes)
{
}

Mesh2d::Mesh2d()
{
}

Mesh2d::~Mesh2d()
{
	
}