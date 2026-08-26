#pragma once

#include "platform/glad_context.hpp"
#include "platform/glfw_context.hpp"
#include "platform/window.hpp"
#include "app/engine.hpp"

class Application
{
	GlfwContext _glfw;
	Window _window;
	GladContext _glad;
	Engine _engine;

public:
	void run();
};
