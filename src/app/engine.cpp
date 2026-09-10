#include "app/engine.hpp"
#include "glm/ext/vector_float3.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/trigonometric.hpp>
#include <array>
#define GLM_ENABLE_EXPERIMENTAL // Needed for string_cast.hpp
#include <glm/gtx/string_cast.hpp>

#include "loader/resource_manager.hpp"
#include "app/frame.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "time.hpp"
#include "scene/label.hpp"


Engine::Engine(Window& window)
    : _window(window)
{
}

Engine::~Engine()
{
}

void Engine::init()
{
	_initRenderSettings();
	_initWorld();
	_initGUI();
}

void Engine::_initRenderSettings() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::_initWorld()
{
	const float aspectRatio = static_cast<float>(_state.resolution.x) /
	                          static_cast<float>(_state.resolution.y);
	_state.projection =
	    glm::perspective(glm::radians(kFov), aspectRatio, kZNear, kZFar);

	_camera.setPos(glm::vec3(0,0,-3));

	ResourceManager& rm = ResourceManager::instance();
	const Shader* shaderPtr = rm.get<Shader>(ResourceId::SHADER_CHUNK);
	const Texture* texturePtr = rm.get<Texture>(ResourceId::TEXTURE_BLOCK_COBBLESTONE);

	_chunk = Chunk({0, 0, 0}, shaderPtr, texturePtr);
	_chunk.build();
}

void Engine::_initGUI()
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

	// NOTE : dynamic cast for cleaness, not needed
	_root.append(LABEL_FRAMERATE, dynamic_cast<Node *>(frameLabel));
	_root.append(LABEL_POSITION, dynamic_cast<Node *>(positionLabel));
	_root.append(LABEL_RESOLUTION, dynamic_cast<Node *>(resolutionLabel));

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

	Label* frameLabel = 		dynamic_cast<Label *>((_root)[LABEL_FRAMERATE]);
	Label* positionLabel = 		dynamic_cast<Label *>((_root)[LABEL_POSITION]);
	Label* resolutionLabel = 	dynamic_cast<Label *>((_root)[LABEL_RESOLUTION]);

	if (frame.input.toggleInfo)
	{
		frameLabel->toggleVisible();
		positionLabel->toggleVisible();
		resolutionLabel->toggleVisible();
	}
	if (frameLabel->getVisible())
	{
		std::string framerate = "Framerate : " + std::to_string(timeinfo::getFramerate(frame.dt));
		frameLabel->setText(framerate);
	}
	if (positionLabel->getVisible())
	{
		std::string position = "Position : " + glm::to_string(_camera.getPos());
		positionLabel->setText(position);
	}
	if (resolutionLabel->getVisible())
	{
		std::string resolution = "Resolution : " + glm::to_string(_window.getRes());
		resolutionLabel->setText(resolution);
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
	// as a parameter for the draw() function

	ResourceManager& rm = ResourceManager::instance();
	const Shader* shader = rm.get<Shader>(ResourceId::SHADER_CHUNK);
	shader->use();
	shader->setUniform<const glm::mat4&>("projection", _state.projection);
	shader->setUniform<const glm::mat4&>("view", _state.view);

	_chunk.draw();
}

void Engine::_renderControl()
{
	glDisable(GL_DEPTH_TEST);

	_root.recursiveDraw();
}

void Engine::_render()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_render3d();
	_renderControl();

	_window.swapBuffers();
}
