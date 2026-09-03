# include "shader.hpp"

# include <glad/glad.h>
# include <iostream>
# include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>


template <typename T>
void Shader::setUniform(const std::string &name, T value) const
{
	GLint loc = glGetUniformLocation(_id, name.c_str());

	if constexpr (std::is_same_v<T, bool>)
		glUniform1i(loc, static_cast<int>(value));
	else if constexpr (std::is_same_v<T, int>)
		glUniform1i(loc, value);
	else if constexpr (std::is_same_v<T, float>)
		glUniform1f(loc, value);
	else if constexpr (std::is_same_v<T, const glm::ivec3&>)
		glUniform3iv(loc, 1, glm::value_ptr(value));
	else if constexpr (std::is_same_v<T, const glm::vec3&>)
		glUniform3fv(loc, 1, glm::value_ptr(value));
	else if constexpr (std::is_same_v<T, const glm::mat4&>)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	else
		throw std::runtime_error("Shader: Uniform of non implemented type");
}