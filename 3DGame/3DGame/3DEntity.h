#pragma once
#include <glm\glm.hpp>

using namespace glm;

class 3DEntity
{
public:

	virtual vec2 getPosition() = 0;
	virtual vec2 setPosition(vec2 pos) = 0;


	virtual void update() = 0;
	virtual void draw(mat4 matrix) = 0;
};