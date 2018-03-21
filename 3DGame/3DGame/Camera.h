#pragma once

#include "3DEntity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#define DEG_TO_RADIAN 0.017453293

class Camera{
private:
	//vec3 targetPos;
	vec3 eye;
	vec3 at;
	vec3 up;
public:
	Camera(vec3 targetPos, vec3 eye, vec3 up) : at(targetPos), eye(eye), up(up) {
		//at = targetPos;
	}
	virtual ~Camera() {}

	void update();
	mat4 transform(mat4 mat);
	
};