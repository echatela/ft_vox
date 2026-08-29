#pragma once

#include "app/frame.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

static constexpr float kYaw = -90.0f;
static constexpr float kPitch = 0.0f;
static constexpr float kSpeed = 5.0f;
static constexpr float kSensitivity = 0.1f;
static constexpr float kSprintMultiplier = 20.0f;

class Camera
{
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	float _yaw;
	float _pitch;

	float _movementSpeed;
	float _mouseSensitivity;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
	       glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = kYaw,
	       float pitch = kPitch);

	glm::mat4 getViewMatrix() const;

	void processInput(const InputIntent& input, float dt);

private:
	void updateVectors();
};
