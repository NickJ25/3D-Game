#pragma once
#include <glm\glm.hpp>

#include <GL/glew.h>

using namespace glm;

class Entity
{
public:

	virtual vec3 getPosition() = 0;
	virtual vec3 setPosition(vec3 pos) = 0;


	virtual void update() = 0;
	virtual mat4 draw(mat4 matrix, GLuint shaderProgram) = 0;
};