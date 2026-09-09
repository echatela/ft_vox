#pragma once

#include <map>

#include "platform/window.hpp"
#include "world/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "world/chunk.hpp"
#include "scene/control.hpp"

constexpr float kZNear = 0.1f;
constexpr float kZFar = 1000.0f;
constexpr float kFov = 80.0f;
constexpr int   kWidth = 800;
constexpr int   kHeight = 600;

static constexpr const glm::vec3 kColorWhite = glm::vec3(0.9, 0.9, 0.9);
static constexpr const glm::vec3 kColorRed = glm::vec3(1.0, 0.0, 0.0);

static constexpr auto kVert = "shaders/control_vert.glsl";
static constexpr auto kFrag = "shaders/control_frag.glsl";

enum CONTROL_ID {CONTROL_FRAMERATE, CONTROL_POSITION, CONTROL_RESOLUTION};

struct State
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::ivec2 resolution = {kWidth, kHeight};

	State()
	    : model(1.0f),
	      view(1.0f),
	      projection(1.0f)
	{
	}
};

class Engine
{
	Window&     _window;

	State   _state;
	Camera  _camera;
	Chunk   _chunk;

	std::map<CONTROL_ID, Control*> controlTree;

public:
	Engine(Window& window);

	void initGUI();
	void loop();

private:

	void _processEvents(Frame &frame);

	void _update(const Frame& frame);
	void _updateGUI(const Frame& frame);

	void _render();
	void _render3d();
	void _renderControl();

public:	

	Engine();
	~Engine();
};
