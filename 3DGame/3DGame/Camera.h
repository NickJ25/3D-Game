#pragma once

#include "Entity.h"
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
	GLfloat rotation = 0;
	GLfloat cameraDistance = 5; // Default distance from player
public:
	Camera(vec3 targetPos, vec3 up, GLfloat startRotation) : at(targetPos), eye(eye), up(up), rotation(startRotation){
		//at = targetPos;
	}
	virtual ~Camera() {}

	GLfloat getRotation();
	void setRotation(GLfloat rotation);

	void init();
	void update();
	mat4 draw(vec3 at);
	
};