#pragma once

#include "node.hpp"
#include "transform.hpp"
#include "mesh_2d.hpp"
#include "material.hpp"

class Control : public Node {

	protected:

		ControlTransform	_transform;
		Mesh2d				_mesh;
		Material			_material;

		virtual void _draw(RenderContext& context) const;

	public:

		const ControlTransform&	getTransform() const;
		void                    setTransform(const ControlTransform& transform);
		const Mesh2d&			getMesh() const;
		void					setMesh(const Mesh2d& mesh);


		void			 setPos(glm::vec2 pos);
		const glm::vec2& getPos() const;

				Control();
		virtual ~Control();
};
