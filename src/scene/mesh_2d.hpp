#pragma once

# include <glm/vec2.hpp>
# include <vector>

class Mesh2d
{

	protected:

		unsigned int _VAO = 0;
		unsigned int _VBO = 0;
		unsigned int _EBO = 0;
		bool	     _activeBuffers = false;
		
		std::vector<glm::vec2> 		_coords;
		std::vector<unsigned int> 	_indexes;

	public:

		const std::vector<glm::vec2>& 	 getCoords() const;
		const std::vector<unsigned int>& getIndexes() const;

		void generateGPUBuffers();
		void bind() const;
		
		void operator=(const Mesh2d &other);

		Mesh2d(std::vector<glm::vec2> coords, std::vector<unsigned int> indexes);
		Mesh2d();
		virtual ~Mesh2d();
};

