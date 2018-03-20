#include "Camera.h"

void Camera::update()
{
	
}

mat4 Camera::transform(mat4 mat)
{
	//at = targetPos;
	eye = glm::vec3(
		20 * cos(20 * DEG_TO_RADIAN)*cos(20*DEG_TO_RADIAN) + at.x,
		20 * sin(20 * DEG_TO_RADIAN) * cos(20 * DEG_TO_RADIAN) + at.y,
		20 * sin(20 * DEG_TO_RADIAN) + at.z);


	return glm::lookAt(eye, at, up);
}
