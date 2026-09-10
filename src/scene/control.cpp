#include "control.hpp"

void Control::draw() const
{
	if (!_visible)
		return ;
	_material.shader->use();
	_material.texture->bind(0);
	_material.shader->setUniform<int>("myTexture", 0);
	_material.shader->setUniform<const glm::vec2 &>("resolution", {2560, 1440});
	_material.shader->setUniform<const glm::vec2 &>("modelPos", getPos());
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

Control::Control()
{
}

Control::~Control()
{
}
