#include "shader.hpp"

#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

static std::string getShaderCode(const char* path)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Shader: File not successfully read");

	std::ostringstream ss;
	ss << file.rdbuf();

	return ss.str();
}

static unsigned int compileShader(const char* shaderCode, int type)
{
	unsigned int shader;
	int          success;
	char         infoLog[512];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		throw std::runtime_error("Shader: Compilation failed\n" +
		                         std::string(infoLog));
	}
	return shader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode = getShaderCode(vertexPath);
	std::string fragmentCode = getShaderCode(fragmentPath);

	unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
	unsigned int fragment =
	    compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

	_id = glCreateProgram();
	glAttachShader(_id, vertex);
	glAttachShader(_id, fragment);
	glLinkProgram(_id);

	int  success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_id, 512, nullptr, infoLog);
		throw std::runtime_error("Shader: Linking failed\n" +
		                         std::string(infoLog));
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(_id);
}

////////////////////////////////////////////////////////////////////////////////

void Shader::use()
{
	glUseProgram(_id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE,
	                   glm::value_ptr(value));
}
