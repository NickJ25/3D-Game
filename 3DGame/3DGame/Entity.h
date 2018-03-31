#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <string>
#include "AABB.h"
#include "md2model.h"

#define DEG_TO_RADIAN 0.017453293

using namespace glm;
using namespace std;

class Entity
{
protected:
	rt3d::materialStruct material = {
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
	{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
	2.0f // shininess
	};
public:


	virtual vec3 getPosition() = 0;
	virtual void setPosition(vec3 pos) = 0;
	virtual AABB* getCollision() = 0;

	virtual void init(GLuint texture) = 0;
	virtual void update() = 0;
	virtual void draw(mat4 matrix, GLuint shaderProgram) = 0;
};