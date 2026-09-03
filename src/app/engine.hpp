#pragma once

#include "platform/glad_context.hpp"
#include "platform/glfw_context.hpp"
#include "platform/window.hpp"
#include "world/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "world/chunk.hpp"
#include "printer.hpp"

static constexpr float kZNear = 0.1f;
static constexpr float kZFar = 1000.0f;
static constexpr float kFov = 80.0f;
static constexpr int kWidth = 800;
static constexpr int kHeight = 600;

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


/*TODO: 
* 3 possibilites :
*	- comme ca
*	- glfw & glad directement dans window
*	- application creer window en smart ptr et move a engine
*/
class Engine
{
	GlfwContext   _glfw;
	Window        _window;
	GladContext   _glad;

	Texture       _texture;
	Shader        _shader;
	State         _state;
	Camera        _camera;
	Printer       _printer;
	Chunk         _chunk;

	Frame	      _frame;

	unsigned int  _vao;

private:

	void _processInputs();
	void _update();
	void _render();

public:	

	void loop();
	Engine();
};
