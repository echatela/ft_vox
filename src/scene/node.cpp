#include "node.hpp"

#include <stdexcept>

void	Node::recursiveDraw(RenderContext context) const
{
	if (!_process)
		return;
	
	_draw(context);
	for (auto &node : _tree)
	{
		node.second->recursiveDraw(context);
	}
}

void Node::append(const std::string &id, Node *ptr)
{
	_tree.insert({id, ptr});
}

// draw rajoute un argument pour la relativite du mvmt

void Node::_draw(RenderContext& context) const
{
	(void)context;
}

void Node::setProcess(bool process)
{
	_process = process;
}

bool Node::getProcess() const
{
	return (_process);
}

void Node::toggleProcess()
{
	_process = !_process;
}


Node* Node::operator[](const std::string& id)
{
	auto i = _tree.find(id);

	if (i == _tree.end())
		throw std::runtime_error("Node[] tried to access a non existant node");
	
	return (_tree.find(id)->second);
}


Node::~Node()
{
	for (const auto &node : _tree)
	{
		delete node.second;
	}
}