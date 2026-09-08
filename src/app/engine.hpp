#pragma once

#include "platform/window.hpp"
#include "world/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "world/chunk.hpp"
#include "printer.hpp"

constexpr float kZNear = 0.1f;
constexpr float kZFar = 1000.0f;
constexpr float kFov = 80.0f;
constexpr int   kWidth = 800;
constexpr int   kHeight = 600;

struct State
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::ivec2 resolution = {kWidth, kHeight};

	State()
	    : model(1.0f),
	      view(1.0f),
	      projection(1.0f)
	{
	}
};

class Engine
{
	Window&     _window;

	Texture _texture;
	Shader  _shader;
	State   _state;
	Camera  _camera;
	Printer _printer;
	Chunk   _chunk;

public:
	Engine(Window& window);

	void loop();

private:
	void _processEvents(Frame& frame);
	void _update(const Frame& frame);
	void _render(const Frame& frame);
};
