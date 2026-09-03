#include "app/engine.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "time.hpp"

#define GLM_ENABLE_EXPERIMENTAL // Needed for string_cast.hpp
#include "glm/gtx/string_cast.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Engine::Engine()
    : _texture("assets/block/cobblestone.png"),
      _shader("shaders/vert.glsl", "shaders/frag.glsl"),
      _camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f)
{
	const float aspectRatio =
	    static_cast<float>(_state.resolution.x) / static_cast<float>(_state.resolution.y);
	_state.projection =
	    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);

	_chunk.build();
}

void Engine::loop()
{
	while (_window.shouldClose() == false)
	{
		_processInputs();
		_update();
		_render();
	}
}

void Engine::_processInputs()
{
	_window.pollEvents();

	if (_window.isKeyPressed(GLFW_KEY_ESCAPE) == true)
		_window.setShouldClose();

	_frame.input.forward = _window.isKeyPressed(GLFW_KEY_W);
	_frame.input.backward = _window.isKeyPressed(GLFW_KEY_S);
	_frame.input.right = _window.isKeyPressed(GLFW_KEY_D);
	_frame.input.left = _window.isKeyPressed(GLFW_KEY_A);
	_frame.input.sprint = _window.isKeyPressed(GLFW_KEY_LEFT_SHIFT);

	_window.consumeCursorOffset(&_frame.input.xOffset, &_frame.input.yOffset);
}

void Engine::_update()
{
	_frame.dt = timeinfo::deltaTime();
	_frame.resolution = _window.getRes();

	if (_frame.resolution != _state.resolution)
	{
		float aspectRatio;

		_state.resolution = _frame.resolution;
		aspectRatio = static_cast<float>(_state.resolution.x) /
		              static_cast<float>(_state.resolution.y);
		_state.projection =
		    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);
	}

	_camera.processInput(_frame.input, _frame.dt);
	_state.view = _camera.getViewMatrix();
}

void Engine::_render()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//render 3D
	glEnable(GL_DEPTH_TEST);

	_shader.use();
	_texture.bind(0);
	_shader.setUniform<int>("texture1", 0);

	_shader.setUniform<const glm::mat4&>("projection", _state.projection);
	_shader.setUniform<const glm::mat4&>("view", _state.view);
	
	_chunk.draw(_shader);

	//render UI
	glDisable(GL_DEPTH_TEST);

	std::string framerate =  "Framerate : " + std::to_string(timeinfo::getFramerate(_frame.dt));
	std::string position =   "Position : " + glm::to_string(_camera.getPos());
	std::string resolution = "Resolution : " + glm::to_string(_window.getRes());

	_printer.print(framerate, glm::vec2(10, 10), 12, _window.getRes(), glm::vec3(0.9, 0.9, 0.9));
	_printer.print(position, glm::vec2(10, 26), 12, _window.getRes(), glm::vec3(0.9, 0.9, 0.9));
	_printer.print(resolution, glm::vec2(10, 42), 12, _window.getRes(), glm::vec3(0.9, 0.9, 0.9));

	_window.swapBuffers();
}
