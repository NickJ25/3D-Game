#include "AABB.h"

vec3 AABB::getPosition()
{
	return position;
}

void AABB::setPosition(vec3 newPos)
{
	position = newPos;
}

vec3 AABB::getSize()
{
	return vec3(rx,ry,rz); // Returns the Width, Height and Depth
}
