#include "glad_context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

GladContext::GladContext()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");

	glEnable(GL_DEPTH_TEST);
}
