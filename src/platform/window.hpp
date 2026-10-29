#pragma once

#include <array>
#include <glm/vec2.hpp>

struct GLFWwindow;

class Window
{
	GLFWwindow* _window;

	glm::ivec2 _resolution = {800, 600};

	std::array<bool, 1024> _keys{false};

	float _cursorOffsetX = 0.0f;
	float _cursorOffsetY = 0.0f;
	float _cursorLastX = 0.0f;
	float _cursorLastY = 0.0f;

public:
	Window();
	~Window();

	bool shouldClose() const;
	void setShouldClose();

	int               getWidth() const;
	int               getHeight() const;
	const glm::ivec2& getRes() const;

	const std::array<bool, 1024>& getKeys() const;

	void swapBuffers();
	void pollEvents();

	bool isKeyPressed(int key);
	void consumeCursorOffset(float* offsetX, float* offsetY);

private:
	static void cursor_pos_callback(GLFWwindow* window, double xpos,
	                                double ypos);
	static void framebuffer_size_callback(GLFWwindow* window, int width,
	                                      int height);
	static void key_callback(GLFWwindow* window, int key, int scancode,
	                         int action, int mods);
};
