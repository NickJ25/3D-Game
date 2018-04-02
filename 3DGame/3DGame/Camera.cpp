#include "Camera.h"

GLfloat Camera::getRotation()
{
	return rotation;
}

void Camera::setRotation(GLfloat newRotation)
{
	rotation = newRotation;
}

void Camera::init()
{
}

void Camera::update()
{
	
}

mat4 Camera::draw(vec3 at)
{
	// rotation amount (r = rotation amount), - 270 so player starts facing forward.
	GLfloat r_cam = rotation - 270;

	// calculate unit vector [x, y] on unit circle using cos, sin of angle
	vec3 offset(cameraDistance * cos(r_cam*DEG_TO_RADIAN), cameraDistance *0.5, cameraDistance* sin(r_cam*DEG_TO_RADIAN));

	//at = playerPos; // Look at the player's position

					// eye (Camera's position) + the rotation
	eye = at + offset;
	return glm::lookAt(eye, at, up);
}
  