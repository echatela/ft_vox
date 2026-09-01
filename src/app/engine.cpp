#include "app/engine.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Engine::Engine()
    : _texture("assets/texture_base.jpg"),
      _shader("shaders/vert.glsl", "shaders/frag.glsl"),
      _camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f)
{
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

	_texture.bind(0);
	_shader.use();

	_shader.setMat4("projection", _state.projection);
	_shader.setMat4("view", _state.view);

	_chunk.draw(_shader);
}
