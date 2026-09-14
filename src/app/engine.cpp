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
#include "render/a_texture.hpp"
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

	_camera.setPos(glm::vec3(0, 0, -3));

	ResourceManager& rm = ResourceManager::instance();
	const Shader*    shaderPtr = rm.get<Shader>(ResourceId::SHADER_CHUNK);
	const ATexture*   texturePtr =
	    rm.get<ATexture>(ResourceId::TEXTURE_BLOCKS);

	Chunk* chunk = new Chunk({0, 0, 0}, shaderPtr, texturePtr);
	chunk->build();

	// _root.append(CHUNK, chunk);

}

void Engine::_initGUI()

{
	Node* menu = new Control();

	Label* frameLabel = new Label("", 24, kColorWhite);
	frameLabel->setPos({10, 10});
	frameLabel->setProcess(false);

	Label* positionLabel = new Label("", 24, kColorWhite);
	positionLabel->setPos({10, 45});
	positionLabel->setProcess(false);

	Label* resolutionLabel = new Label("", 24, kColorWhite);
	resolutionLabel->setPos({10, 80});
	resolutionLabel->setProcess(false);

	menu->append(NodeId::LABEL_FRAMERATE, frameLabel);
	menu->append(NodeId::LABEL_POSITION, positionLabel);
	menu->append(NodeId::LABEL_RESOLUTION, resolutionLabel);
	
	_root.append(NodeId::MENU, menu);

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
	Control* menu = dynamic_cast<Control *>(_root[MENU]);

	Label* frameLabel = 		dynamic_cast<Label *>((*menu)[LABEL_FRAMERATE]);
	Label* positionLabel = 		dynamic_cast<Label *>((*menu)[LABEL_POSITION]);
	Label* resolutionLabel = 	dynamic_cast<Label *>((*menu)[LABEL_RESOLUTION]);

	if (frame.input.toggleInfo)
	{
		frameLabel->toggleProcess();
		positionLabel->toggleProcess();
		resolutionLabel->toggleProcess();
	}
	if (frameLabel->getProcess())
	{
		std::string framerate = "Framerate : " + std::to_string(timeinfo::getFramerate(frame.dt));
		frameLabel->setText(framerate);
	}
	if (positionLabel->getProcess())
	{
		std::string position = "Position : " + glm::to_string(_camera.getPos());
		positionLabel->setText(position);
	}
	if (resolutionLabel->getProcess())
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

	_camera.processInput(frame.input, frame.dt);
	_state.view = _camera.getViewMatrix();

	_updateGUI(frame);
}

// void Engine::_render3d(const Frame& frame)
// {
// 	// render 3D
// 	glEnable(GL_DEPTH_TEST);

// 	// TODO : The shader used here and in _chunk.draw() are the same (as exactly the same, we use a ptr)
// 	// Since chunk now has its texture, I moved the binding in _chunk.draw() function.
// 	//
// 	// Since we are in the _render3d(), maybe every 3D object should take the projection/view matrix
// 	// as a parameter for the draw() function

// 	ResourceManager& rm = ResourceManager::instance();
// 	const Shader* shader = rm.get<Shader>(ResourceId::SHADER_CHUNK);
// 	shader->use();
// 	shader->setUniform<const glm::mat4&>("projection", _state.projection);
// 	shader->setUniform<const glm::mat4&>("view", _state.view);

// 	_chunk.draw();
// }

void Engine::_renderControl()
{
	glDisable(GL_DEPTH_TEST);

	RenderContext context;

	context._rect[2] = _window.getRes()[0];
	context._rect[3] = _window.getRes()[1];

	_root.recursiveDraw(context);
}

void Engine::_render()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// _render3d();
	_renderControl();

	_window.swapBuffers();
}

// void Engine::_render3d()
// {
// 	glEnable(GL_DEPTH_TEST);

// 	_chunk.draw(_state.projection * _state.view);
// }


