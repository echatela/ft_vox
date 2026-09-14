#pragma once

#include <map>
#include "world/camera.hpp"

struct RenderContext
{
	glm::mat4          mat3D; //Repere 3D
	unsigned int       rect[4] = {};
};

enum NodeId : int
{
	//3D
	CHUNK_MANAGER,
	CHUNK,

	// CONTROLS
	MENU,

	LABEL_FRAMERATE,
	LABEL_POSITION,
	LABEL_RESOLUTION
};

class Node 
{
	private:

		std::multimap<NodeId, Node*>	_tree;
		bool                    		_process = true;

		virtual void	_draw(RenderContext& context) const;
	
	public:
	
		void recursiveDraw(RenderContext context) const;
		void append(NodeId id, Node *ptr);

		void	setProcess(bool process);
		bool	getProcess() const;
		void	toggleProcess();

		Node * operator[](NodeId id);

		Node() = default;
		virtual ~Node();

};