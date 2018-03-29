#pragma once
#include <glm\glm.hpp>
#include <GL/glew.h>
#include <string>
#include "AABB.h"
#include "md2model.h"
#include "rt3dObjLoader.h"

#define DEG_TO_RADIAN 0.017453293

using namespace glm;
using namespace std;

class Entity
{
public:

	virtual vec3 getPosition() = 0;
	virtual void setPosition(vec3 pos) = 0;
	virtual AABB* getCollision() = 0;

	virtual void init(GLuint texture) = 0;
	virtual void update() = 0;
	virtual void draw(mat4 matrix, GLuint shaderProgram) = 0;
};