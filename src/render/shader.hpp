#pragma once

#include <string>

#include "loader/resource.hpp"

class Shader : public Resource
{
	unsigned int _id;

public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use() const;

	template <typename T>
	void setUniform(const std::string& name, T value) const;
};

#include "shader.tpp"
