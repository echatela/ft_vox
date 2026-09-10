#pragma once

# include <string>
# include <glm/vec3.hpp>

# include "control.hpp"

struct Letter {

	unsigned int offset;
	unsigned int width;

	Letter(unsigned int _offset, unsigned int _width) {
		offset = _offset;
		width = _width;
	}

	Letter() {};
};

class Label : public Control
{
	private :

		std::string  _text;
		unsigned int _size;
		glm::vec3    _color;
		
		void	_constructMesh();

	public :

		virtual void	draw() const override;

		const std::string&	getText() const;
		void                setText(const std::string& text);
		unsigned int        getSize() const;
		void                setSize(const unsigned int size);
		const glm::vec3&	getColor() const;
		void                setColor(const glm::vec3& color);

			Label(std::string text, unsigned int size, glm::vec3 color);
			Label() = delete;
	virtual ~Label();
};