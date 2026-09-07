# include "control.hpp"
# include "glad/glad.h"

void Control::generateGPUBuffers()
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
	glBufferData(GL_ARRAY_BUFFER, _mesh.getCoords().size() * sizeof(glm::vec2), _mesh.getCoords().data(), GL_STATIC_DRAW);
	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.getIndexes().size() * sizeof(unsigned int), _mesh.getIndexes().data(), GL_STATIC_DRAW);

	// layout 0 -> pos
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)0);
	glEnableVertexAttribArray(0) ;
	// layout 1 -> UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void *)sizeof(glm::vec2));
	glEnableVertexAttribArray(1);
}

void Control::draw(const Shader& shader) const
{
	if (!_visible)
		return ;
	shader.use(); 
	_texture.bind(0);
	shader.setUniform<int>("myTexture", 0);
	shader.setUniform<const glm::vec2 &>("resolution", {2560, 1440});
	shader.setUniform<const glm::vec2 &>("modelPos", getPos());
	glDrawElements(GL_TRIANGLES, _mesh.getIndexes().size(), GL_UNSIGNED_INT, (void *)0);
}


const ControlTransform&	Control::getTransform() const
{
	return _transform;
}

void Control::setTransform(const ControlTransform &transform)
{
	_transform = transform;
}

const Mesh2d& Control::getMesh() const
{
	return _mesh;
}

void Control::setMesh(const Mesh2d& mesh)
{
	_mesh = mesh;
}

Texture& Control::getTexture()
{
	return _texture;
}

void Control::setTexture(const Texture& texture)
{
	_texture = texture;
}

unsigned int Control::getVAO() const
{
	return (_VAO);
}

void	Control::setPos(glm::vec2 pos)
{
	_transform.position = pos;
}

const glm::vec2& Control::getPos() const
{
	return (_transform.position);
}

void	Control::toggleVisible()
{
	_visible = !_visible;
}

void	Control::setVisible(bool visible)
{
	_visible = visible;
}

bool	Control::getVisible() const
{
	return (_visible);
}

Control::Control() : _texture()
{
	glGenVertexArrays(1, &_VAO);
}

Control::~Control()
{
	glDeleteVertexArrays(1, &_VAO);
	if (_activeBuffers)
	{
		glDeleteBuffers(1, &_VBO);
		glDeleteBuffers(1, &_EBO);
	}
}