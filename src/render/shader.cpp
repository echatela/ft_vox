#include "shader.hpp"

#include <fstream>
#include <glad/glad.h>
#include <sstream>
#include <stdexcept>

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

void Shader::use() const
{
	glUseProgram(_id);
}
