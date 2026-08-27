#pragma once

struct GLFWwindow;

class Window
{
	GLFWwindow* _window;

	int _width = 0;
	int _height = 0;

	float _cursorLastX = -1.f;
	float _cursorLastY = -1.f;
	float _cursorOffsetX = 0.0f;
	float _cursorOffsetY = 0.0f;

public:
	Window();
	~Window();

	bool shouldClose() const;
	void setShouldClose();
	void getFramebufferSize(int* width, int* height) const;

	void swapBuffers();
	void pollEvents();

	bool isKeyPressed(int key);
	void consumeCursorOffset(float* offsetX, float* offsetY);

private:
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void framebufferSizeCallback(GLFWwindow* window, int width,
	                                    int height);
};
