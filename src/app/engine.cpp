#include "app/engine.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "time.hpp"
#include <array>

#define GLM_ENABLE_EXPERIMENTAL // Needed for string_cast.hpp
#include "glm/gtx/string_cast.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Engine::Engine(Window& window)
    : _window(window),
      _texture("assets/block/cobblestone.png"),
      _shader("shaders/chunk_vert.glsl", "shaders/chunk_frag.glsl"),
      _camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f)
{
	const float aspectRatio = static_cast<float>(_state.resolution.x) /
	                          static_cast<float>(_state.resolution.y);
	_state.projection =
	    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);

	_chunk.build();
}

void Engine::loop()
{
	Frame frame;

	while (!_window.shouldClose())
	{
		_processEvents(frame);
		_update(frame);
		_render(frame);
	}
}

void Engine::_processEvents(Frame& frame)
{
	_window.pollEvents();

	const std::array<bool, 1024>& keys = _window.getKeys();
	InputIntent&                  input = frame.input;

	frame.dt = timeinfo::deltaTime();
	frame.resolution = _window.getRes();

	if (keys[GLFW_KEY_ESCAPE])
		_window.setShouldClose();

	input.forward = keys[GLFW_KEY_W];
	input.backward = keys[GLFW_KEY_S];
	input.right = keys[GLFW_KEY_D];
	input.left = keys[GLFW_KEY_A];
	input.sprint = keys[GLFW_KEY_LEFT_SHIFT];

	_window.consumeCursorOffset(&input.xOffset, &input.yOffset);
}

void Engine::_update(const Frame& frame)
{
	if (frame.resolution != _state.resolution)
	{
		float aspectRatio;

		_state.resolution = frame.resolution;
		aspectRatio = static_cast<float>(_state.resolution.x) /
		              static_cast<float>(_state.resolution.y);
		_state.projection =
		    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);
	}

	_camera.processInput(frame.input, frame.dt);
	_state.view = _camera.getViewMatrix();
}

// TODO: la fonction render ne devrait se charger que de render donc la creation
// et l'update des string ne devrait pas se faire ici mais dans update. le const
// Frame& frame n'aura donc plus lieu d'etre
void Engine::_render(const Frame& frame)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render 3D
	glEnable(GL_DEPTH_TEST);

	_shader.use();
	_texture.bind(0);
	_shader.setUniform<int>("texture1", 0);

	_shader.setUniform<const glm::mat4&>("projection", _state.projection);
	_shader.setUniform<const glm::mat4&>("view", _state.view);

	_chunk.draw(_shader);

	// render UI
	glDisable(GL_DEPTH_TEST);

	std::string framerate =
	    "Framerate : " + std::to_string(timeinfo::getFramerate(frame.dt));
	std::string position = "Position : " + glm::to_string(_camera.getPos());
	std::string resolution = "Resolution : " + glm::to_string(_window.getRes());

	_printer.print(framerate, glm::vec2(10, 10), 12, _window.getRes(),
	               glm::vec3(0.9, 0.9, 0.9));
	_printer.print(position, glm::vec2(10, 26), 12, _window.getRes(),
	               glm::vec3(0.9, 0.9, 0.9));
	_printer.print(resolution, glm::vec2(10, 42), 12, _window.getRes(),
	               glm::vec3(0.9, 0.9, 0.9));

	_window.swapBuffers();
}
