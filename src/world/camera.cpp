#include "world/camera.hpp"
#include "app/frame.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : _position(position),
      _worldUp(worldUp),
      _yaw(yaw),
      _pitch(pitch),
      _movementSpeed(kSpeed),
      _mouseSensitivity(kSensitivity)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_position, _position + _front, _up);
}

const glm::vec3 &Camera::getPos() const
{
	return _position;
}

void Camera::processInput(const InputIntent& input, float dt)
{
	float     velocity = _movementSpeed * dt;
	glm::vec3 dir(0.0f);

	if (input.sprint)
		velocity *= kSprintMultiplier;

	if (input.forward == true)
		dir += _front;
	if (input.backward == true)
		dir -= _front;
	if (input.right == true)
		dir += _right;
	if (input.left == true)
		dir -= _right;
	if (glm::length(dir) > 0.0001f)
		_position += glm::normalize(dir) * velocity;

	_yaw += input.xOffset * kSensitivity;
	_pitch += input.yOffset * kSensitivity;

	if (_pitch > 89.9f)
		_pitch = 89.9f;
	if (_pitch < -89.9f)
		_pitch = -89.9f;

	updateVectors();
}

void Camera::updateVectors()
{
	glm::vec3 front;
	
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
