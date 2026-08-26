#pragma once

struct GLFWwindow;

class Window
{
	GLFWwindow* _window;

	int _width = 0;
	int _height = 0;

	double _scrollOffset = 0.0;

public:
	Window();
	~Window();

	bool shouldClose() const;
	void setShouldClose();
	void getFramebufferSize(int* width, int* height) const;

	void swapBuffers();
	void pollEvents();

	bool   isKeyPressed(int key);
	double consumeScroll();

private:
	static void scrollCallback(GLFWwindow* window, double xoffset,
	                           double yoffset);
	static void framebufferSizeCallback(GLFWwindow* window, int width,
	                                    int height);
};
