#include "glfw_context.hpp"

#include <GLFW/glfw3.h>

GlfwContext::GlfwContext()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GlfwContext::~GlfwContext()
{
	glfwTerminate();
}
