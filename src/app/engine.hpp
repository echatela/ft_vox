#pragma once

#include "render/texture_2d_array.hpp"
#include "world/camera.hpp"
#include "app/frame.hpp"
#include "render/shader.hpp"
#include "world/chunk.hpp"

#include "glm/ext/matrix_float4x4.hpp"

static constexpr float kZNear = 0.1f;
static constexpr float kZFar = 1000.0f;
static constexpr float kFov = 80.0f;
static constexpr int   kWidth = 800;
static constexpr int   kHeight = 600;

struct EngineState
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	int width = kWidth;
	int height = kHeight;

	EngineState()
	    : model(1.0f),
	      view(1.0f),
	      projection(1.0f)
	{
	}
};

class Engine
{
	EngineState _state;

	Texture2DArray _texBlock;
	Shader         _shaderBlock;
	Chunk          _chunk;
	Camera         _camera;

public:
	Engine();

	void update(const Frame& frame);
	void render();
};
