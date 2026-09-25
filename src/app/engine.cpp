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
#include "render/texture_2d.hpp"
#include "render/shader.hpp"
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
	_initGUI();
	_initWorld();
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

	_camera.setPos(glm::vec3(0, 140, 0));

	// ResourceManager& rm = ResourceManager::instance();
	// const Shader*    shaderPtr = rm.get<Shader>(ResourceId::SHADER_CHUNK);
	// const ATexture*   texturePtr =
	//     rm.get<ATexture>(ResourceId::TEXTURE_BLOCKS);

	ChunkManager* chunk = new ChunkManager();
	// chunk->build();

	_root.append("chunk_manager", chunk);

}

constexpr auto kHotbarSelectorPadding = 4;

void Engine::_initGuiInfo()
{
	Control* info = new Control();

	Label* frameLabel = new Label("", 24, kColorWhite);
	frameLabel->setPos({10, 10});
	
	Label* positionLabel = new Label("", 24, kColorWhite);
	positionLabel->setPos({10, 45});
	
	Label* resolutionLabel = new Label("", 24, kColorWhite);
	resolutionLabel->setPos({10, 80});
	
	Label* chunkCountLabel = new Label("", 24, kColorWhite);
	chunkCountLabel->setPos({10, 150});
	
	info->append("label_framerate", frameLabel);
	info->append("label_position",  positionLabel);
	info->append("label_resolution", resolutionLabel);
	info->append("label_chunkcount", chunkCountLabel);

	_root.append("info", info);
}

void Engine::_initGuiHud()
{
	Control* hud = new Control();

	ResourceManager &rm = ResourceManager::instance();

	Control* hotbar = new Control(rm.get<Shader>(ResourceId::SHADER_CONTROL),
                                  rm.get<Texture2D>(ResourceId::TEXTURE_HOTBAR),
								  glm::vec2(_window.getWidth() / 3, 
								            _window.getHeight() / 15));

	Control* hotbarSelector = new Control(
		rm.get<Shader>(ResourceId::SHADER_CONTROL),
    	rm.get<Texture2D>(ResourceId::TEXTURE_HOTBAR_SELECTOR),
		glm::vec2(_window.getWidth() / (3 * 9) + kHotbarSelectorPadding,
		          _window.getHeight() / 15 + kHotbarSelectorPadding));		  
	
	hotbar->setAnchor(Anchor::BOTTOM_CENTER);
	hotbarSelector->setAnchor(Anchor::CENTER_LEFT);
	hotbarSelector->setPos({-kHotbarSelectorPadding / 2, 0});

	hotbar->append("hotbar_selector", hotbarSelector);
	hud->append("hotbar", hotbar);

	_root.append("hud", hud);
}

void Engine::_initGUI()
{
	_initGuiInfo();
	_initGuiHud();
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

	input.numpad1 = keys[GLFW_KEY_1];
	input.numpad2 = keys[GLFW_KEY_2];
	input.numpad3 = keys[GLFW_KEY_3];
	input.numpad4 = keys[GLFW_KEY_4];
	input.numpad5 = keys[GLFW_KEY_5];
	input.numpad6 = keys[GLFW_KEY_6];
	input.numpad7 = keys[GLFW_KEY_7];
	input.numpad8 = keys[GLFW_KEY_8];
	input.numpad9 = keys[GLFW_KEY_9];

	input.forward = keys[GLFW_KEY_W];
	input.backward = keys[GLFW_KEY_S];
	input.right = keys[GLFW_KEY_D];
	input.left = keys[GLFW_KEY_A];
	input.up = keys[GLFW_KEY_SPACE];
	input.sprint = keys[GLFW_KEY_LEFT_SHIFT];

	if (keys[GLFW_KEY_F5])
	{
		if (!input.infoKeyHold)
		{
			input.infoToggle = true;
			input.infoKeyHold = true;
		}
		else
		{
			input.infoToggle = false;
		}
	}
	else if (input.infoKeyHold && !keys[GLFW_KEY_F5])
	{
		input.infoKeyHold = false;
	}
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

void Engine::_updateGuiInfoFramerate(const Frame& frame, Label* label)
{
	int        currFramerate;
	static int lastFramerate;
	static int tick;
	
	currFramerate = timeinfo::getFramerate(frame.dt);

	if (tick > 10)
		tick = 0;
	
	if (currFramerate < kLowFramerate)
		label->setColor(kColorRed);
	else if (currFramerate <= lastFramerate)
	{
		tick ++;
		label->setColor(kColorOrange);
	}
	else if (!tick)
		label->setColor(kColorWhite);
	
	lastFramerate = currFramerate;

	std::string framerate = "Framerate : " + std::to_string(currFramerate);
	label->setText(framerate);
}

void Engine::_updateGuiInfoPosition(Label* label)
{
	std::string camPos = 	std::to_string((int)_camera.getPos().x) + ", " + 
							std::to_string((int)_camera.getPos().y) + ", " +
							std::to_string((int)_camera.getPos().z);

	std::string chunkPos = 	std::to_string(std::floor(_camera.getPos().x / 16)) + ", " +
							std::to_string(std::floor(_camera.getPos().z / 16));

	std::string position = "Position : (" + camPos + ") | (" + chunkPos + ")";
	label->setText(position);
}

void Engine::_updateGuiInfoResolution(Label* label)
{
	std::string resolution = "Resolution : " + glm::to_string(_window.getRes());
	label->setText(resolution);
}

void Engine::_updateGuiInfoChunkCount(Label* label)
{
	ChunkManager* chunkManager = dynamic_cast<ChunkManager *>(_root["chunk_manager"]);
	std::string chunkcount = "Chunk count : " + std::to_string(chunkManager->getSize());
	label->setText(chunkcount);
}

void Engine::_updateGuiInfo(const Frame& frame)
{
	Control* info =             dynamic_cast<Control *>(_root["info"]);

	if (frame.input.infoToggle)
		info->toggleProcess();
	if (!info->getProcess())
		return ;

	Label* framerateLabel = 	dynamic_cast<Label *>((*info)["label_framerate"]);
	Label* positionLabel = 		dynamic_cast<Label *>((*info)["label_position"]);
	Label* resolutionLabel = 	dynamic_cast<Label *>((*info)["label_resolution"]);
	Label* chunkCountLabel = 	dynamic_cast<Label *>((*info)["label_chunkcount"]);

	_updateGuiInfoFramerate(frame, framerateLabel);
	_updateGuiInfoPosition(positionLabel);
	_updateGuiInfoResolution(resolutionLabel);
	_updateGuiInfoChunkCount(chunkCountLabel);
}

void Engine::_updateGuiHud(const Frame& frame)
{
	Control* hud =              dynamic_cast<Control *>(_root["hud"]);
	Control* hotbarSelector =	dynamic_cast<Control *>((*(*hud)["hotbar"])["hotbar_selector"]);

	for (int i = 0; i < 9; i++)
	{
		if (*(&frame.input.numpad1 + sizeof(bool) * i))
		{
			hotbarSelector->setPos({-(kHotbarSelectorPadding / 2) + (hotbarSelector->getTransform().rect.x -(kHotbarSelectorPadding)) * i, 0});
			break ;
		}
	}
}

void Engine::_updateGUI(const Frame& frame)
{

	_updateGuiInfo(frame);
	_updateGuiHud(frame);
	
}

void Engine::_render() const
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderContext context;

	context.mat3D = _state.projection * _state.view;
	context.res = _window.getRes();
	context.rect[2] = context.res.x;
	context.rect[3] = context.res.y;

	_root.recursiveDraw(context);

	_window.swapBuffers();
}
