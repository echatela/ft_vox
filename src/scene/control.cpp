#include "control.hpp"

void Control::_draw(RenderContext& context) const
{
	context.rect[0] += getPos().x;
	context.rect[2] += getPos().x;
	context.rect[1] += getPos().y;
	context.rect[3] += getPos().y;

	if (_material.shader == nullptr || _material.texture == nullptr)
		return ;

	glm::vec2 rect = {context.rect[2] - context.rect[0], context.rect[3] - context.rect[1]};
	// TODO : marging/anchor to calculate offset
	glm::vec2 offset = {context.rect[0], context.rect[1]};

	_material.shader->use();
	_material.texture->bind(0);
	_material.shader->setUniform<int>("myTexture", 0);
	_material.shader->setUniform<const glm::vec2 &>("offset", offset);
	_material.shader->setUniform<const glm::vec2 &>("rect", rect);

	glDisable(GL_DEPTH_TEST);
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

Control::Control()
{
}

Control::~Control()
{
}
