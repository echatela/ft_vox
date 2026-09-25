#include "control.hpp"

void Control::_draw(RenderContext& context) const
{
	if (_material.shader == nullptr || _material.texture == nullptr)
		return ;

	glm::vec2 offset;

	switch (_transform.anchor)
	{
		case Anchor::TOP_LEFT :
		{
			offset = {context.rect[0], context.rect[1]};
			break;
		}
		case Anchor::TOP_CENTER :
		{
			offset = {context.rect[0] + (context.rect[2] - context.rect[0]) / 2 - _transform.rect.x / 2, context.rect[1]};
			break;
		}
		case Anchor::TOP_RIGHT :
		{
			offset = {context.rect[0] + (context.rect[2] - context.rect[0]) - _transform.rect.x, context.rect[1]};
			break;
		}
		case Anchor::CENTER_LEFT : 
		{
			offset = {context.rect[0], context.rect[1] + (context.rect[3] - context.rect[1]) / 2 - _transform.rect.y / 2};
			break;
		}
		case Anchor::CENTER :
		{
			offset = {context.rect[0] + ((context.rect[2] - context.rect[0]) / 2) - _transform.rect.x / 2, context.rect[1] + (context.rect[3] - context.rect[1]) / 2 - _transform.rect.y / 2};
			break;
		}
		case Anchor::CENTER_RIGHT :
		{
			offset = {context.rect[0] + (context.rect[2] - context.rect[0]) - _transform.rect.x, context.rect[1] + (context.rect[3] - context.rect[1]) / 2 - _transform.rect.y / 2};
			break;
		}
		case Anchor::BOTTOM_LEFT :
		{
			offset = {context.rect[0], (context.rect[3] - _transform.rect.y)};
			break;
		}
		case Anchor::BOTTOM_CENTER :
		{
			offset = {context.rect[0] + (context.rect[2] - context.rect[0]) / 2 - _transform.rect.x / 2, (context.rect[3] - _transform.rect.y)};
			break;
		}
		case Anchor::BOTTOM_RIGHT :
		{
			offset = {context.rect[0] + (context.rect[2] - context.rect[0]) - _transform.rect.x, (context.rect[3] - _transform.rect.y)};
			break;
		}
		default:
			__builtin_unreachable();
	}

	offset += getPos();

	context.rect[0] = offset.x;
	context.rect[2] = offset.x + _transform.rect.x;
	context.rect[1] = offset.y;
	context.rect[3] = offset.y + _transform.rect.y;

	_mesh.bind();
	_material.shader->use();
	_material.texture->bind(0);
	_material.shader->setUniform<int>("myTexture", 0);
	_material.shader->setUniform<const glm::vec2 &>("offset", offset);
	_material.shader->setUniform<const glm::vec2 &>("res", context.res);

	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, _mesh.getIndexes().size(), GL_UNSIGNED_INT, (void *)0);
}

void Control::_constructMesh()
{
	std::vector<glm::vec2> 		coords;
	std::vector<unsigned int> 	indexes;

	//v1
	coords.push_back(glm::vec2(0, 0));
	coords.push_back(glm::vec2(0, 0));
	//v2
	coords.push_back(glm::vec2(_transform.rect.x, 0));
	coords.push_back(glm::vec2(1, 0));
	//v3
	coords.push_back(glm::vec2(_transform.rect.x, -_transform.rect.y));
	coords.push_back(glm::vec2(1, 1));
	//v4
	coords.push_back(glm::vec2(0, -_transform.rect.y));
	coords.push_back(glm::vec2(0, 1));

	for (const int vertIndex : {0, 1, 3, 1, 2, 3})
		indexes.push_back(vertIndex);

	setMesh({coords, indexes});
};

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

void	Control::setAnchor(Anchor anchor)
{
	_transform.anchor = anchor;
}

Anchor Control::getAnchor() const
{
	return (_transform.anchor);
}

Control::Control(const Shader* shader, const ATexture* texture, const glm::vec2& textureRes)
{
	_material.shader = shader;
	_material.texture = texture;

	_transform.rect = textureRes;
	_constructMesh();
}


Control::Control()
{
}

Control::~Control()
{
}
