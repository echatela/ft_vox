#pragma once

#include "app/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"

static constexpr float kFov = 80.0f;
static constexpr int kWidth = 800;
static constexpr int kHeight = 600;

struct State
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	int width = kWidth;
	int height = kHeight;

	State()
	    : model(1.0f),
	      view(1.0f),
	      projection(1.0f)
	{
	}
};

class Engine
{
	Texture      _texture;
	Shader       _shader;
	State        _state;
	unsigned int _vao;
	Camera       _camera;

public:
	Engine();

	void update(const Frame& frame);
	void render();

private:
	void setupMesh();
};
