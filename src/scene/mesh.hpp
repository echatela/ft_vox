#pragma once

# include <glm/vec2.hpp>
# include <vector>

class Mesh2d
{

	private:
		
		std::vector<glm::vec2> 		_coords;
		std::vector<unsigned int> 	_indexes;

	public:

		const std::vector<glm::vec2>& 	 getCoords() const;
		const std::vector<unsigned int>& getIndexes() const;
		
		Mesh2d(std::vector<glm::vec2> coords, std::vector<unsigned int> indexes);
		Mesh2d();
		virtual ~Mesh2d();
};

