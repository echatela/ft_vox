#pragma once

#include <glm/vec2.hpp>
#include "platform/glad_context.hpp"
#include "platform/glfw_context.hpp"

struct GLFWwindow;

class Window
{
	GLFWwindow* _window;

	glm::ivec2 _resolution = {800, 600};

	bool  _cursorInitialized = false;
	float _cursorLastX = 0.0f;
	float _cursorLastY = 0.0f;
	float _cursorOffsetX = 0.0f;
	float _cursorOffsetY = 0.0f;

public:
	Window();
	~Window();

	bool shouldClose() const;
	void setShouldClose();

	int getWidth() const;
	int getHeight() const;
	const glm::ivec2 &getRes() const;

	void swapBuffers();
	void pollEvents();

	bool isKeyPressed(int key);
	void consumeCursorOffset(float* offsetX, float* offsetY);

private:
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void framebufferSizeCallback(GLFWwindow* window, int width,
	                                    int height);
};
