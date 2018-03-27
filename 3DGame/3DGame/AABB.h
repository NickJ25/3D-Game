#pragma once

#include <glm\glm.hpp>
#include <GL/glew.h>

using namespace glm;

class AABB {
private:
	vec3 position;
	GLfloat rx, ry, rz;
public:
	AABB(vec3 pos, GLfloat rx, GLfloat ry, GLfloat rz) : position(pos), rx(rx), ry(ry),rz(rz) {

	}
	virtual ~AABB() {};

	vec2 getPosition();

};