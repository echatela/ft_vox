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

#include "app/frame.hpp"
#include "render/shader.hpp"
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

	_camera.setPos(glm::vec3(0, 90, 0));

	// ResourceManager& rm = ResourceManager::instance();
	// const Shader*    shaderPtr = rm.get<Shader>(ResourceId::SHADER_CHUNK);
	// const ATexture*   texturePtr =
	//     rm.get<ATexture>(ResourceId::TEXTURE_BLOCKS);

	ChunkManager* chunk = new ChunkManager();
	// chunk->build();

	_root.append("chunk_manager", chunk);

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

	Label* chunkCountLabel = new Label("", 24, kColorWhite);
	chunkCountLabel->setPos({10, 150});
	chunkCountLabel->setProcess(false);

	menu->append("label_framerate", frameLabel);
	menu->append("label_position", positionLabel);
	menu->append("label_resolution", resolutionLabel);
	menu->append("label_chunkcount", chunkCountLabel);
	
	frameLabel->toggleProcess();
	positionLabel->toggleProcess();
	resolutionLabel->toggleProcess();
	chunkCountLabel->toggleProcess();
	// menu->toggleProcess();

	_root.append("menu", menu);

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

	_updateWorld();
	_updateGUI(frame);
}

void Engine::_updateWorld()
{
	((ChunkManager*)_root["chunk_manager"])->updateChunks(_camera.getPos());
}

constexpr auto kLowFramerate = 60;

void Engine::_updateGUI(const Frame& frame)
{
	Control* menu = dynamic_cast<Control *>(_root["menu"]);
	ChunkManager* chunkManager = dynamic_cast<ChunkManager *>(_root["chunk_manager"]);

	static int lastframerate;
	static int tick;

	Label* frameLabel = 		dynamic_cast<Label *>((*menu)["label_framerate"]);
	Label* positionLabel = 		dynamic_cast<Label *>((*menu)["label_position"]);
	Label* resolutionLabel = 	dynamic_cast<Label *>((*menu)["label_resolution"]);
	Label* chunkCountLabel = 	dynamic_cast<Label *>((*menu)["label_chunkcount"]);

	if (frame.input.toggleInfo)
	{
		menu->toggleProcess();
	}
	if (menu->getProcess())
	{
		int frames = timeinfo::getFramerate(frame.dt);
		if (tick > 10)
			tick = 0;
		if (frames < kLowFramerate)
		{
			frameLabel->setColor(kColorRed);
		}
		else if (frames <= lastframerate)
		{
			tick ++;
			frameLabel->setColor(kColorOrange);
		}
		
		else if (!tick)
			frameLabel->setColor(kColorWhite);

		lastframerate = frames;

		std::string framerate = "Framerate : " + std::to_string(frames);
		frameLabel->setText(framerate);

		std::string camPos = 	std::to_string((int)_camera.getPos().x) + ", " + 
								std::to_string((int)_camera.getPos().y) + ", " +
								std::to_string((int)_camera.getPos().z);

		std::string chunkPos = 	std::to_string(std::floor(_camera.getPos().x / 16)) + ", " +
								std::to_string(std::floor(_camera.getPos().z / 16));

		std::string position = "Position : (" + camPos + ") | (" + chunkPos + ")";

		positionLabel->setText(position);

		std::string resolution = "Resolution : " + glm::to_string(_window.getRes());
		resolutionLabel->setText(resolution);

		std::string chunkcount = "Chunk count : " + std::to_string(chunkManager->getSize());
		chunkCountLabel->setText(chunkcount);

	}
}

void Engine::_render() const
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderContext context;

	context.mat3D = _state.projection * _state.view;

	context.rect[2] = _window.getRes()[0];
	context.rect[3] = _window.getRes()[1];

	_root.recursiveDraw(context);

	_window.swapBuffers();
}
