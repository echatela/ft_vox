#include "node.hpp"

#include <stdexcept>

void	Node::recursiveDraw() const
{
	draw();
	for (auto &node : _tree)
	{
		node.second->recursiveDraw();
	}
}

void Node::append(NodeId id, Node *ptr)
{
	_tree[id] = ptr;
}

void Node::draw() const
{
}

Node* Node::operator[](NodeId id)
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