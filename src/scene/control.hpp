#pragma once

#include "transform.hpp"
#include "mesh.hpp"
#include "material.hpp"

class Control {

	protected:

		bool _visible = true;

		// UNE CLASSE ?
		unsigned int _VAO;
		unsigned int _VBO;
		unsigned int _EBO;
		bool	     _activeBuffers = false;

		ControlTransform	_transform;
		Mesh2d				_mesh;
		Material			_material;
	
	public:

		const ControlTransform&	getTransform() const;
		void                    setTransform(const ControlTransform& transform);
		const Mesh2d&			getMesh() const;
		void					setMesh(const Mesh2d& mesh);
		unsigned int			getVAO() const;
		void					toggleVisible();
		void					setVisible(bool visible);
		bool					getVisible() const;

		void generateGPUBuffers();
		virtual void draw() const;

		void			 setPos(glm::vec2 pos);
		const glm::vec2& getPos() const;

				Control();
		virtual ~Control();
};