#include "Camera.h"

void Camera::update()
{
}

mat4 Camera::transform(mat4 mat)
{

	return glm::lookAt(eye, at, up);
}
