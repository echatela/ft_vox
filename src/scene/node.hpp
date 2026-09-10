#pragma once

#include <map>

enum NodeId
{
	LABEL_FRAMERATE,
	LABEL_POSITION,
	LABEL_RESOLUTION
};

class Node 
{

	std::map<NodeId, Node*>	_tree;

	virtual void	draw() const;
	
	public:
	
		void recursiveDraw() const;
		void append(NodeId id, Node *ptr);

		Node * operator[](NodeId id);

		Node() = default;
		virtual ~Node();

};