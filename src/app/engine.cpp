#include "app/engine.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include "render/shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

const std::vector<std::string> kblockFiles = {
    "assets/block/stone.png", "assets/block/dirt.png",
    "assets/block/cobblestone.png", "assets/block/oak_planks.png"};

Engine::Engine()
    : _texBlock(kblockFiles, 16, 16, true),
      _shaderBlock("shaders/vert.glsl", "shaders/frag.glsl"),
      _chunk(_shaderBlock, _texBlock),
      _camera(glm::vec3(0.0f, 0.0f, -3.0f))
{
	_texBlock.loadFromFiles(kblockFiles, 16, 16, true);

	const float aspectRatio =
	    static_cast<float>(_state.width) / static_cast<float>(_state.height);
	_state.projection =
	    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);

	_chunk.build();
}

void Engine::update(const Frame& frame)
{
	if (frame.width != _state.width || frame.height != _state.height)
	{
		float aspectRatio;

		_state.width = frame.width;
		_state.height = frame.height;
		aspectRatio = static_cast<float>(_state.width) /
		              static_cast<float>(_state.height);
		_state.projection =
		    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);
	}

	_camera.processInput(frame.input, frame.dt);
	_state.view = _camera.getViewMatrix();
}

void Engine::render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shaderBlock.setMat4("projection", _state.projection);
	_shaderBlock.setMat4("view", _state.view);

	_chunk.draw();
}
