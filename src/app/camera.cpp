#include "app/camera.hpp"
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

void Camera::processInput(const InputIntent& input, float dt)
{
	if (input.moved)
	{
		float velocity = _movementSpeed * dt;
		if (input.sprint)
			velocity *= kSprintMultiplier;

		_position += _bodyFront * velocity * input.move.z;
		_position += _right * velocity * input.move.x;
		_position += _worldUp * velocity * input.move.y;
	}

	_yaw += input.look.x * kSensitivity;
	_pitch += input.look.y * kSensitivity;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	updateVectors();
}

void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);
	_bodyFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	_right = glm::normalize(glm::cross(_bodyFront, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}
