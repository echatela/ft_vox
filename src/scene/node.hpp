#pragma once

#include <string>
#include <map>
#include "world/camera.hpp"

struct RenderContext
{
	glm::mat4          mat3D; //Repere 3D
	unsigned int       rect[4] = {};
	glm::vec2		   res = {};
};

class Node 
{
	protected:

		std::map<std::string, Node*>	_tree;
		bool                    		_process = true;

		virtual void	_draw(RenderContext& context) const;
	
	public:
	
		void recursiveDraw(RenderContext context) const;
		void append(const std::string &id, Node *ptr);

		void	setProcess(bool process);
		bool	getProcess() const;
		void	toggleProcess();

		Node * operator[](const std::string &id);

		Node() = default;
		virtual ~Node();

};