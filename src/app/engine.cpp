#include "app/engine.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

#include "loader/resource_manager.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/trigonometric.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "time.hpp"
#include "scene/label.hpp"

#define GLM_ENABLE_EXPERIMENTAL // Needed for string_cast.hpp
#include "glm/gtx/string_cast.hpp"



Engine::Engine(Window& window)
    : _window(window),
      _camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f)
{
	const float aspectRatio = static_cast<float>(_state.resolution.x) /
	                          static_cast<float>(_state.resolution.y);
	_state.projection =
	    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);

	ResourceManager& rm = ResourceManager::instanciate();
	const Shader* shaderPtr = rm.get<Shader>(RESOURCE_ID::RES_SHADER_CHUNK);
	const Texture* texturePtr = rm.get<Texture>(RESOURCE_ID::RES_TEXTURE_BLOCK_COBBLESTONE);
	_chunk = Chunk({0, 0, 0}, shaderPtr, texturePtr);
	_chunk.build();
}

// controlTree deviendra une classe, mettre cela dans le destructeur de control ?
Engine::~Engine()
{
	for (const std::pair<const CONTROL_ID, Control*> &control : controlTree)
	{
		delete control.second;
	}
}

void Engine::initGUI()
{

	Label *frameLabel = new Label("", 24, kColorWhite);
	frameLabel->setPos({10, 10});
	frameLabel->setVisible(false);

	Label *positionLabel = new Label("", 24, kColorWhite);
	positionLabel->setPos({10, 45});
	positionLabel->setVisible(false);

	Label *resolutionLabel = new Label("", 24, kColorWhite);
	resolutionLabel->setPos({10, 80});
	resolutionLabel->setVisible(false);

	controlTree[CONTROL_FRAMERATE] = frameLabel;
	controlTree[CONTROL_POSITION] = positionLabel;
	controlTree[CONTROL_RESOLUTION] = resolutionLabel;
}

void Engine::loop()
{
	Frame frame;

	while (!_window.shouldClose())
	{
		_processEvents(frame);
		_update(frame);
		_render();
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
	input.toggleInfo = keys[GLFW_KEY_F5];

	_window.consumeCursorOffset(&input.xOffset, &input.yOffset);
}

void Engine::_updateGUI(const Frame& frame)
{
	if (frame.input.toggleInfo)
	{
		controlTree[CONTROL_FRAMERATE]->toggleVisible();
		controlTree[CONTROL_POSITION]->toggleVisible();
		controlTree[CONTROL_RESOLUTION]->toggleVisible();
	}
	if (controlTree[CONTROL_FRAMERATE]->getVisible())
	{
		std::string framerate = "Framerate : " + std::to_string(timeinfo::getFramerate(frame.dt));
		((Label *)controlTree[CONTROL_FRAMERATE])->setText(framerate);
	}
	if (controlTree[CONTROL_POSITION]->getVisible())
	{
		std::string position = "Position : " + glm::to_string(_camera.getPos());
		((Label *)controlTree[CONTROL_POSITION])->setText(position);
	}
	if (controlTree[CONTROL_RESOLUTION]->getVisible())
	{
		std::string resolution = "Resolution : " + glm::to_string(_window.getRes());
		((Label *)controlTree[CONTROL_RESOLUTION])->setText(resolution);
	}
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

	_updateGUI(frame);

	_camera.processInput(frame.input, frame.dt);
	_state.view = _camera.getViewMatrix();
}

void Engine::_render3d()
{
	// render 3D
	glEnable(GL_DEPTH_TEST);

	// TODO : The shader used here and in _chunk.draw() are the same (as exactly the same, we use a ptr)
	// Since chunk now has its texture, I moved the binding in _chunk.draw() function.
	//
	// Since we are in the _render3d(), maybe every 3D object should take the projection/view matrix
	// as a parameter for the draw()
	// and

	ResourceManager& rm = ResourceManager::instanciate();
	const Shader* shader = rm.get<Shader>(RESOURCE_ID::RES_SHADER_CHUNK);
	const Texture* texture = rm.get<Texture>(RESOURCE_ID::RES_TEXTURE_BLOCK_COBBLESTONE);
	shader->use();
	shader->setUniform<const glm::mat4&>("projection", _state.projection);
	shader->setUniform<const glm::mat4&>("view", _state.view);

	_chunk.draw();
}

void Engine::_renderControl()
{
	glDisable(GL_DEPTH_TEST);

	for (const std::pair<const CONTROL_ID, Control*> &control : controlTree)
	{
		control.second->draw();
	}
}

void Engine::_render()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_render3d();
	_renderControl();

	_window.swapBuffers();
}
