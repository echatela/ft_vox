#pragma once

# include <map>
# include "glm/vec3.hpp"
# include "glm/mat4x4.hpp"

#include "platform/window.hpp"
#include "scene/node.hpp"
#include "world/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "world/chunk.hpp"

constexpr float kZNear = 0.1f;
constexpr float kZFar = 1000.0f;
constexpr float kFov = 80.0f;
constexpr int   kWidth = 800;
constexpr int   kHeight = 600;

constexpr glm::vec3 kColorWhite = glm::vec3(0.9, 0.9, 0.9);
constexpr glm::vec3 kColorRed = glm::vec3(1.0, 0.0, 0.0);

constexpr auto kVert = "shaders/control_vert.glsl";
constexpr auto kFrag = "shaders/control_frag.glsl";

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
	Window& _window;

	State  _state;
	Camera _camera;
	Chunk  _chunk;

	Node _root;

public:
	Engine(Window& window);

	void init();
	void loop();

private:
	void _initRenderSettings() const;
	void _initWorld();
	void _initGUI();

	void _processEvents(Frame& frame);

	void _update(const Frame& frame);
	void _updateGUI(const Frame& frame);

	void _render();
	void _render3d();
	void _renderControl();

public:
	Engine();
	~Engine();
};
